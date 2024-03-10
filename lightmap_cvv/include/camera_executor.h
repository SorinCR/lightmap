//
// Created by valen on 3/9/2024.
//

#ifndef LIGHTMAP_CV_CAMERA_EXECUTOR_H
#define LIGHTMAP_CV_CAMERA_EXECUTOR_H

#include <opencv2/opencv.hpp>
#include "my_blob.h"
using namespace cv;
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
    const cv::Scalar in_line_color = cv::Scalar(0, 255.0, 0);
    const cv::Scalar out_line_color = cv::Scalar(0, 0, 255.0);
    int carCount = 0;
    std::vector<Blob> blobs;
    bool blnFirstFrame = true;
    int frameCount = 2;
    cv::Mat imgFrame1;
    cv::Mat imgFrame2;
    // eCAL::protobuf::CPublisher<pb::spp::SPPolylineModelData> m_lineSender;
};
#endif //LIGHTMAP_CV_CAMERA_EXECUTOR_H
