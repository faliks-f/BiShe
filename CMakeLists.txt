cmake_minimum_required(VERSION 3.23)
project(BiShe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets REQUIRED)

set(PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_PREFIX_PATH /opt/Qt5.14.2/5.14.2)

include_directories(${OpenCV_INCLUDE_DIRS} ${PATH}/include)
aux_source_directory(${PATH}/src SRC_DIRS)
add_executable(BiShe ${SRC_DIRS} ${PATH}/include/MainWindow.h)

target_link_libraries(BiShe ${OpenCV_LIBS}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)

set(CMAKE_CXX_FLAGS "-g -Wall -pthread -O3")

