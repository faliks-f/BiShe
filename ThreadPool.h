//
// Created by faliks on 1/26/23.
//

#ifndef DAY6_THREADPOOL_H
#define DAY6_THREADPOOL_H

#include "condition_variable"
#include "functional"
#include "future"
#include "mutex"
#include "queue"
#include "thread"
#include "vector"

class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex taskMutex;
    std::condition_variable conditionVariable;
    bool stop;

public:
    explicit ThreadPool(int size = 10);

    ~ThreadPool();

    //    void add(const std::function<void()> &func);
    template<class F, class... Args>
    auto add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

template<class F, class... Args>
auto ThreadPool::add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using ReturnType = typename std::result_of<F(Args...)>::type;
    auto task =
            std::make_shared<std::packaged_task<ReturnType()>>(
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<ReturnType> res = (*task).get_future();
    {
        std::unique_lock<std::mutex> lock(taskMutex);
        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace([task]() { (*task)(); });
    }
    conditionVariable.notify_one();
    return res;
}

#endif  // DAY6_THREADPOOL_H
