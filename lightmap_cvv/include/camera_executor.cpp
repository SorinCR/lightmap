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
    while((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000)<62000) {
        double t =  (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
        t = (int)(t / (1000.0 / 30.0));
        std::stringstream ss;
        ss << t;
        std::string str = ss.str();
        cv::Mat inputRgbMat;
        cv::Mat resized_down;
        switch(m_camera){
            case 0:
                inputRgbMat = cv::imread("camera0\\" + str + ".jpg");
                cv::line(inputRgbMat, cv::Point(300, 643),cv::Point(475, 790), out_line_color, 5);
                cv::line(inputRgbMat, cv::Point(970, 110),cv::Point(1010, 160), out_line_color, 5);
                cv::line(inputRgbMat, cv::Point(310, 80),cv::Point(430, 53), out_line_color, 5);
                cv::line(inputRgbMat, cv::Point(1030, 775),cv::Point(1308, 730), out_line_color, 5);
                //resize down
                resize(inputRgbMat, resized_down, cv::Size(960, 540), cv::INTER_LINEAR);
                // cv::imshow("Test",resized_down);

                break;

            case 1:
                inputRgbMat = cv::imread("camera1\\" + str + ".jpg");
                cv::line(inputRgbMat, cv::Point(405, 508),cv::Point(450, 685), in_line_color, 5);
                //resize down
                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
                 // cv::imshow("Test",resized_down);
                break;
            case 2:
                inputRgbMat = cv::imread("camera2\\" + str + ".jpg");
                //resize down
                cv::line(inputRgbMat, cv::Point(1350, 70),cv::Point(1400, 120), in_line_color, 5);
                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
                // cv::imshow("Test",resized_down);
                break;
            case 3:
                inputRgbMat = cv::imread("camera3\\" + str + ".jpg");
                cv::line(inputRgbMat, cv::Point(690, 530),cv::Point(780, 540), in_line_color, 5);
                //resize down

                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
                // cv::imshow("Test",resized_down);
                break;
            case 4:
                inputRgbMat = cv::imread("camera4\\" + str + ".jpg");
                cv::line(inputRgbMat, cv::Point(700, 590),cv::Point(730, 837), in_line_color, 5);
                //resize down
                resize(inputRgbMat, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
//                for(int x = 0;x<10;x++)
//                    for(int y=0;y<30;y++)
//                        resized_down.at<cv::Vec3b>(x,y)=cv::Vec3b(0,0,255);
                //cv::imshow("Test",resized_down);
                break;

        }
        ImageBuffer buffer = ImageBuffer::getInstance();
        buffer.setFrame(resized_down, m_camera);
        buffer.show(m_camera);
    }
}
