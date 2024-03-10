cmake_minimum_required(VERSION 3.23)
project(lightmap_cv)

set(CMAKE_CXX_STANDARD 23)
set(ENV{OPENCV_DIR} "D:\\opencv\\build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lightmap_cv main.cpp
        include/camera_provider.h
        include/image_buffer.h
        include/camera_executor.h
        include/camera_executor.cpp
        include/my_blob.h
        include/my_blob.cpp
        include/helper.h
)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

target_link_libraries(lightmap_cv ${OpenCV_LIBS})

