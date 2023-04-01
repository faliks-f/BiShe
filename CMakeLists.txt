cmake_minimum_required(VERSION 3.10)
project(BiShe)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
set(PATH ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS}  ${PATH}/include)
aux_source_directory(${PATH}/src SRC_DIRS)
add_executable(BiShe ${SRC_DIRS})
target_link_libraries(BiShe ${OpenCV_LIBS})

set(CMAKE_CXX_FLAGS "-g -Wall -pthread -O3")

