//
// Created by valen on 3/9/2024.
//

#ifndef LIGHTMAP_CV_CAMERA_EXECUTOR_H
#define LIGHTMAP_CV_CAMERA_EXECUTOR_H

#include <opencv2/opencv.hpp>
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
    std::vector<std::pair<cv::Point, cv::Point>> in_lines = {{{75, 130}, {80, 175}}};
    cv::Scalar in_line_color = cv::Scalar(0, 255, 0);
    cv::Scalar out_line_color = cv::Scalar(0, 0, 255);
    std::vector<std::pair<cv::Point, cv::Point>> out_lines = {{{140 + 960, 320}, { 200 + 960, 395}}};
   // eCAL::protobuf::CPublisher<pb::spp::SPPolylineModelData> m_lineSender;
};
#endif //LIGHTMAP_CV_CAMERA_EXECUTOR_H
