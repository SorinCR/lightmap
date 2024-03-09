//
// Created by valen on 3/9/2024.
//
#include "camera_executor.h"
#include "image_buffer.h"


CameraExecutor::CameraExecutor(int camera) : m_camera(camera)
{
    start = std::clock();

    std::cout << camera << " camera created" << std::endl;
}
void CameraExecutor::execute() {
    std::cout << "Thread : " << m_camera << std::endl;
    while((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000)<60000) {
        double t =  (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
        t = (int)(t / (1000.0 / 30.0));
        std::stringstream ss;
        ss << t;
        std::string str = ss.str();
        cv::Mat inputRgbMat;
        cv::Mat resized_down;
        switch(m_camera){
            case 0:
                inputRgbMat = cv::imread("camera1\\" + str + ".jpg");
                //resize down
                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
//                for(int x = 0;x<10;x++)
//                    for(int y=0;y<30;y++)
//                        resized_down.at<cv::Vec3b>(x,y)=cv::Vec3b(0,0,255);
                 cv::imshow("Test",resized_down);
                break;
//            case 1:
//                inputRgbMat = cv::imread("camera2\\" + str + ".jpg");
//                //resize down
//                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
//                // cv::imshow("Test",resized_down);
//                break;
//            case 2:
//                inputRgbMat = cv::imread("camera3\\" + str + ".jpg");
//                //resize down
//                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
//                // cv::imshow("Test",resized_down);
//                break;
//            case 3:
//                inputRgbMat = cv::imread("camera4\\" + str + ".jpg");
//                //resize down
//                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
//                // cv::imshow("Test",resized_down);
//                break;
//            case 4:
//                inputRgbMat = cv::imread("cameraMare\\" + str + ".jpg");
//                //resize down
//                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
//                // cv::imshow("Test",resized_down);
//                break;

        }
        ImageBuffer buffer = ImageBuffer::getInstance();
        buffer.setFrame(resized_down, m_camera);
        buffer.show(m_camera);
    }
}
