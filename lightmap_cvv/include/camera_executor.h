//
// Created by valen on 3/9/2024.
//

#ifndef LIGHTMAP_CV_CAMERA_EXECUTOR_H
#define LIGHTMAP_CV_CAMERA_EXECUTOR_H

#include <opencv2/opencv.hpp>

class CameraExecutor
{
public:
    CameraExecutor(int camera);
    void execute();
    void operator()()
    {
        execute();
    }

private:
    int m_camera;
    std::clock_t start;
   // eCAL::protobuf::CPublisher<pb::spp::SPPolylineModelData> m_lineSender;
};
#endif //LIGHTMAP_CV_CAMERA_EXECUTOR_H
