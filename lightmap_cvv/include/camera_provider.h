//
// Created by valen on 3/9/2024.
//
#include "image_buffer.h"
#include "camera_executor.h"
#include <thread>
#include <vector>

#ifndef LIGHTMAP_CV_CAMERA_PROVIDER_H
#define LIGHTMAP_CV_CAMERA_PROVIDER_H
class CameraProvider{
public:
    static void run()
    {
        ImageBuffer& buffer = ImageBuffer::getInstance(); // Assuming getInstance returns a reference

        std::vector<CameraExecutor> executors; // Vector to hold instances of CameraExecutor

        for (int camera = 0; camera < 5; camera++)
        {
            executors.emplace_back(camera); // Assuming CameraExecutor has a constructor that takes an int
        }

        std::vector<std::thread> executor_threads;
        executor_threads.reserve(executors.size());
        for (auto &&executor : executors)
        {
            executor_threads.emplace_back(std::thread(std::ref(executor))); // Pass executor by reference
        }

        for (auto &executor_thread : executor_threads)
        {
            executor_thread.join();
        }
    }
};
#endif //LIGHTMAP_CV_CAMERA_PROVIDER_H
