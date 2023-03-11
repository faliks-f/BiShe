//
// Created by faliks on 1/26/23.
//

#include "ThreadPool.h"

using namespace std;

ThreadPool::ThreadPool(int size) {
  this->stop = false;
  for (int i = 0; i < size; i++) {
    threads.emplace_back(thread([this]() {
      while (true) {
        function<void()> task;
        {
          unique_lock<mutex> lock(taskMutex);
          conditionVariable.wait(lock, [this]() { return stop || !tasks.empty(); });
          if (stop && tasks.empty()) {
            return;
          }
          task = tasks.front();
          tasks.pop();
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    unique_lock<mutex> lock(taskMutex);
    stop = true;
  }
  conditionVariable.notify_all();
  for (auto &th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }
}
