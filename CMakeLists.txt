cmake_minimum_required(VERSION 3.10)
project(BiShe)

set(CMAKE_CXX_STANDARD 17)

add_executable(BiShe main.cpp Uart.cpp Utils.cpp Buffer.cpp ThreadPool.cpp Control.cpp)


set(CMAKE_CXX_FLAGS "-g -Wall -pthread")
