//
// Created by valen on 3/9/2024.
//
#include "camera_executor.h"
#include "image_buffer.h"
#include "helper.h"


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
        std::stringstream sss;
        sss << m_camera;
        std::string strr = sss.str();
        cv::Mat inputRgbMat;
        cv::Mat resized_down;
        imgFrame1=cv::imread("camera"+strr+"\\0.jpg");
        switch(m_camera){
            case 0: {
                inputRgbMat = cv::imread("camera0\\" + str + ".jpg");
                cv::line(inputRgbMat, cv::Point(300, 643), cv::Point(475, 790), out_line_color, 5);
                cv::line(inputRgbMat, cv::Point(970, 110), cv::Point(1010, 160), out_line_color, 5);
                cv::line(inputRgbMat, cv::Point(310, 80), cv::Point(430, 53), out_line_color, 5);
                cv::line(inputRgbMat, cv::Point(1030, 775), cv::Point(1308, 730), out_line_color, 5);
                imgFrame2 = cv::imread("camera0\\" + str + ".jpg");

                cv::Point crossingLine[2];
                crossingLine[0].x = 300;
                crossingLine[0].y = 643;
                crossingLine[1].x = 475;
                crossingLine[1].y = 790;

                std::vector<Blob> currentFrameBlobs;

                cv::Mat imgFrame1Copy = imgFrame1.clone();
                cv::Mat imgFrame2Copy = imgFrame2.clone();

                cv::Mat imgDifference;
                cv::Mat imgThresh;

                cv::cvtColor(imgFrame1Copy, imgFrame1Copy, cv::COLOR_BGR2GRAY);
                cv::cvtColor(imgFrame2Copy, imgFrame2Copy, cv::COLOR_BGR2GRAY);

                cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(7, 7), 0);
                cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(7, 7), 0);

                cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

                cv::threshold(imgDifference, imgThresh, 30, 255.0, cv::THRESH_BINARY);

                // cv::imshow("imgThresh", imgThresh);

                cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
                cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
                cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
                cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

                for (unsigned int i = 0; i < 2; i++) {
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::erode(imgThresh, imgThresh, structuringElement5x5);
                }

                cv::Mat imgThreshCopy = imgThresh.clone();

                std::vector<std::vector<cv::Point> > contours;

                cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

                drawAndShowContours(imgThresh.size(), contours, "imgContours");

                std::vector<std::vector<cv::Point> > convexHulls(contours.size());

                for (unsigned int i = 0; i < contours.size(); i++) {
                    cv::convexHull(contours[i], convexHulls[i]);
                }

                drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

                for (auto &convexHull: convexHulls) {
                    Blob possibleBlob(convexHull);

                    if (possibleBlob.currentBoundingRect.area() > 400 &&
                        possibleBlob.dblCurrentAspectRatio > 0.2 &&
                        possibleBlob.dblCurrentAspectRatio < 4.0 &&
                        possibleBlob.currentBoundingRect.width > 30 &&
                        possibleBlob.currentBoundingRect.height > 30 &&
                        possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                        (cv::contourArea(possibleBlob.currentContour) /
                         (double) possibleBlob.currentBoundingRect.area()) > 0.50) {
                        currentFrameBlobs.push_back(possibleBlob);
                    }
                }

                drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

                if (blnFirstFrame == true) {
                    for (auto &currentFrameBlob: currentFrameBlobs) {
                        blobs.push_back(currentFrameBlob);
                    }
                } else {
                    matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
                }

                drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

                imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

                drawBlobInfoOnImage(blobs, imgFrame2Copy);
                bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, 300, 475, 643, 790, carCount);

                if (blnAtLeastOneBlobCrossedTheLine == true) {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
                } else {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
                }

                drawCarCountOnImage(carCount, imgFrame2Copy);

                //        cv::imshow("imgFrame2Copy", imgFrame2Copy);

                //cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

                // now we prepare for the next iteration

                currentFrameBlobs.clear();

                imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is


                blnFirstFrame = false;
                frameCount++;

                //resize down
                resize(imgFrame2Copy, resized_down, cv::Size(960, 540), cv::INTER_LINEAR);
                // cv::imshow("Test",resized_down);
                break;
            }
            case 1: {
                imgFrame2 = cv::imread("camera1\\" + str + ".jpg");
               // cv::line(inputRgbMat, cv::Point(405, 508), cv::Point(450, 685), in_line_color, 5);
                cv::Point crossingLine[2];
                crossingLine[0].x = 405;
                crossingLine[0].y = 508;
                crossingLine[1].x = 450;
                crossingLine[1].y = 685;

                std::vector<Blob> currentFrameBlobs;

                cv::Mat imgFrame1Copy = imgFrame1.clone();
                cv::Mat imgFrame2Copy = imgFrame2.clone();

                cv::Mat imgDifference;
                cv::Mat imgThresh;

                cv::cvtColor(imgFrame1Copy, imgFrame1Copy, cv::COLOR_BGR2GRAY);
                cv::cvtColor(imgFrame2Copy, imgFrame2Copy, cv::COLOR_BGR2GRAY);

                cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(7, 7), 0);
                cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(7, 7), 0);

                cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

                cv::threshold(imgDifference, imgThresh, 30, 255.0, cv::THRESH_BINARY);

                // cv::imshow("imgThresh", imgThresh);

                cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
                cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
                cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
                cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

                for (unsigned int i = 0; i < 2; i++) {
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::erode(imgThresh, imgThresh, structuringElement5x5);
                }

                cv::Mat imgThreshCopy = imgThresh.clone();

                std::vector<std::vector<cv::Point> > contours;

                cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

                drawAndShowContours(imgThresh.size(), contours, "imgContours");

                std::vector<std::vector<cv::Point> > convexHulls(contours.size());

                for (unsigned int i = 0; i < contours.size(); i++) {
                    cv::convexHull(contours[i], convexHulls[i]);
                }

                drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

                for (auto &convexHull : convexHulls) {
                    Blob possibleBlob(convexHull);

                    if (possibleBlob.currentBoundingRect.area() > 400 &&
                        possibleBlob.dblCurrentAspectRatio > 0.2 &&
                        possibleBlob.dblCurrentAspectRatio < 4.0 &&
                        possibleBlob.currentBoundingRect.width > 30 &&
                        possibleBlob.currentBoundingRect.height > 30 &&
                        possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                        (cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50) {
                        currentFrameBlobs.push_back(possibleBlob);
                    }
                }

                drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

                if (blnFirstFrame == true) {
                    for (auto &currentFrameBlob : currentFrameBlobs) {
                        blobs.push_back(currentFrameBlob);
                    }
                } else {
                    matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
                }

                drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

                imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

                drawBlobInfoOnImage(blobs, imgFrame2Copy);

                bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, 405,450,508,685, carCount);

                if (blnAtLeastOneBlobCrossedTheLine == true) {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
                } else {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
                }

                drawCarCountOnImage(carCount, imgFrame2Copy);

        //        cv::imshow("imgFrame2Copy", imgFrame2Copy);

                //cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

                // now we prepare for the next iteration

                currentFrameBlobs.clear();

                imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is


                blnFirstFrame = false;
                frameCount++;

                //resize down
                resize(imgFrame2Copy, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
                // cv::imshow("Test",resized_down);
                break;
            }
            case 2: {
                imgFrame2 = cv::imread("camera2\\" + str + ".jpg");

                cv::Point crossingLine[2];
                crossingLine[0].x = 1350;
                crossingLine[0].y = 70;
                crossingLine[1].x = 1400;
                crossingLine[1].y = 120;

                std::vector<Blob> currentFrameBlobs;

                cv::Mat imgFrame1Copy = imgFrame1.clone();
                cv::Mat imgFrame2Copy = imgFrame2.clone();

                cv::Mat imgDifference;
                cv::Mat imgThresh;

                cv::cvtColor(imgFrame1Copy, imgFrame1Copy, cv::COLOR_BGR2GRAY);
                cv::cvtColor(imgFrame2Copy, imgFrame2Copy, cv::COLOR_BGR2GRAY);

                cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(7, 7), 0);
                cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(7, 7), 0);

                cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

                cv::threshold(imgDifference, imgThresh, 30, 255.0, cv::THRESH_BINARY);

                // cv::imshow("imgThresh", imgThresh);

                cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
                cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
                cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
                cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

                for (unsigned int i = 0; i < 2; i++) {
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::erode(imgThresh, imgThresh, structuringElement5x5);
                }

                cv::Mat imgThreshCopy = imgThresh.clone();

                std::vector<std::vector<cv::Point> > contours;

                cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

                drawAndShowContours(imgThresh.size(), contours, "imgContours");

                std::vector<std::vector<cv::Point> > convexHulls(contours.size());

                for (unsigned int i = 0; i < contours.size(); i++) {
                    cv::convexHull(contours[i], convexHulls[i]);
                }

                drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

                for (auto &convexHull: convexHulls) {
                    Blob possibleBlob(convexHull);

                    if (possibleBlob.currentBoundingRect.area() > 400 &&
                        possibleBlob.dblCurrentAspectRatio > 0.2 &&
                        possibleBlob.dblCurrentAspectRatio < 4.0 &&
                        possibleBlob.currentBoundingRect.width > 30 &&
                        possibleBlob.currentBoundingRect.height > 30 &&
                        possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                        (cv::contourArea(possibleBlob.currentContour) /
                         (double) possibleBlob.currentBoundingRect.area()) > 0.50) {
                        currentFrameBlobs.push_back(possibleBlob);
                    }
                }

                drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

                if (blnFirstFrame == true) {
                    for (auto &currentFrameBlob: currentFrameBlobs) {
                        blobs.push_back(currentFrameBlob);
                    }
                } else {
                    matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
                }

                drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

                imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

                drawBlobInfoOnImage(blobs, imgFrame2Copy);

                bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, 1350, 1400, 70, 120, carCount);

                if (blnAtLeastOneBlobCrossedTheLine == true) {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
                } else {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
                }

                drawCarCountOnImage(carCount, imgFrame2Copy);

                //        cv::imshow("imgFrame2Copy", imgFrame2Copy);

                //cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

                // now we prepare for the next iteration

                currentFrameBlobs.clear();

                imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is


                blnFirstFrame = false;
                frameCount++;

                //resize down
                resize(imgFrame2Copy, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
                // cv::imshow("Test",resized_down);
                break;
            }
            case 3: {
                imgFrame2 = cv::imread("camera3\\" + str + ".jpg");

                cv::Point crossingLine[2];
                crossingLine[0].x = 690;
                crossingLine[0].y = 530;
                crossingLine[1].x = 780;
                crossingLine[1].y = 540;

                std::vector<Blob> currentFrameBlobs;

                cv::Mat imgFrame1Copy = imgFrame1.clone();
                cv::Mat imgFrame2Copy = imgFrame2.clone();

                cv::Mat imgDifference;
                cv::Mat imgThresh;

                cv::cvtColor(imgFrame1Copy, imgFrame1Copy, cv::COLOR_BGR2GRAY);
                cv::cvtColor(imgFrame2Copy, imgFrame2Copy, cv::COLOR_BGR2GRAY);

                cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(7, 7), 0);
                cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(7, 7), 0);

                cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

                cv::threshold(imgDifference, imgThresh, 30, 255.0, cv::THRESH_BINARY);

                // cv::imshow("imgThresh", imgThresh);

                cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
                cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
                cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
                cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

                for (unsigned int i = 0; i < 2; i++) {
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::erode(imgThresh, imgThresh, structuringElement5x5);
                }

                cv::Mat imgThreshCopy = imgThresh.clone();

                std::vector<std::vector<cv::Point> > contours;

                cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

                drawAndShowContours(imgThresh.size(), contours, "imgContours");

                std::vector<std::vector<cv::Point> > convexHulls(contours.size());

                for (unsigned int i = 0; i < contours.size(); i++) {
                    cv::convexHull(contours[i], convexHulls[i]);
                }

                drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

                for (auto &convexHull: convexHulls) {
                    Blob possibleBlob(convexHull);

                    if (possibleBlob.currentBoundingRect.area() > 400 &&
                        possibleBlob.dblCurrentAspectRatio > 0.2 &&
                        possibleBlob.dblCurrentAspectRatio < 4.0 &&
                        possibleBlob.currentBoundingRect.width > 30 &&
                        possibleBlob.currentBoundingRect.height > 30 &&
                        possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                        (cv::contourArea(possibleBlob.currentContour) /
                         (double) possibleBlob.currentBoundingRect.area()) > 0.50) {
                        currentFrameBlobs.push_back(possibleBlob);
                    }
                }

                drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

                if (blnFirstFrame == true) {
                    for (auto &currentFrameBlob: currentFrameBlobs) {
                        blobs.push_back(currentFrameBlob);
                    }
                } else {
                    matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
                }

                drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

                imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

                drawBlobInfoOnImage(blobs, imgFrame2Copy);

                bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, 690, 780, 530, 540, carCount);

                if (blnAtLeastOneBlobCrossedTheLine == true) {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
                } else {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
                }

                drawCarCountOnImage(carCount, imgFrame2Copy);

                //        cv::imshow("imgFrame2Copy", imgFrame2Copy);

                //cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

                // now we prepare for the next iteration

                currentFrameBlobs.clear();

                imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is


                blnFirstFrame = false;
                frameCount++;

                //resize down
                resize(imgFrame2Copy, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
                // cv::imshow("Test",resized_down);
                break;
            }
            case 4: {
                imgFrame2 = cv::imread("camera4\\" + str + ".jpg");

                cv::Point crossingLine[2];
                crossingLine[0].x = 700;
                crossingLine[0].y = 590;
                crossingLine[1].x = 730;
                crossingLine[1].y = 837;

                std::vector<Blob> currentFrameBlobs;

                cv::Mat imgFrame1Copy = imgFrame1.clone();
                cv::Mat imgFrame2Copy = imgFrame2.clone();

                cv::Mat imgDifference;
                cv::Mat imgThresh;

                cv::cvtColor(imgFrame1Copy, imgFrame1Copy, cv::COLOR_BGR2GRAY);
                cv::cvtColor(imgFrame2Copy, imgFrame2Copy, cv::COLOR_BGR2GRAY);

                cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(7, 7), 0);
                cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(7, 7), 0);

                cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

                cv::threshold(imgDifference, imgThresh, 30, 255.0, cv::THRESH_BINARY);

                // cv::imshow("imgThresh", imgThresh);

                cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
                cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
                cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
                cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

                for (unsigned int i = 0; i < 2; i++) {
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                    cv::erode(imgThresh, imgThresh, structuringElement5x5);
                }

                cv::Mat imgThreshCopy = imgThresh.clone();

                std::vector<std::vector<cv::Point> > contours;

                cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

                drawAndShowContours(imgThresh.size(), contours, "imgContours");

                std::vector<std::vector<cv::Point> > convexHulls(contours.size());

                for (unsigned int i = 0; i < contours.size(); i++) {
                    cv::convexHull(contours[i], convexHulls[i]);
                }

                drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

                for (auto &convexHull: convexHulls) {
                    Blob possibleBlob(convexHull);

                    if (possibleBlob.currentBoundingRect.area() > 400 &&
                        possibleBlob.dblCurrentAspectRatio > 0.2 &&
                        possibleBlob.dblCurrentAspectRatio < 4.0 &&
                        possibleBlob.currentBoundingRect.width > 30 &&
                        possibleBlob.currentBoundingRect.height > 30 &&
                        possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                        (cv::contourArea(possibleBlob.currentContour) /
                         (double) possibleBlob.currentBoundingRect.area()) > 0.50) {
                        currentFrameBlobs.push_back(possibleBlob);
                    }
                }

                drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

                if (blnFirstFrame == true) {
                    for (auto &currentFrameBlob: currentFrameBlobs) {
                        blobs.push_back(currentFrameBlob);
                    }
                } else {
                    matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
                }

                drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

                imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

                drawBlobInfoOnImage(blobs, imgFrame2Copy);
                bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, 700, 730, 590, 837, carCount);

                if (blnAtLeastOneBlobCrossedTheLine == true) {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
                } else {
                    cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
                }

                drawCarCountOnImage(carCount, imgFrame2Copy);

                //        cv::imshow("imgFrame2Copy", imgFrame2Copy);

                //cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

                // now we prepare for the next iteration

                currentFrameBlobs.clear();

                imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is


                blnFirstFrame = false;
                frameCount++;

                //resize down
                resize(imgFrame2Copy, resized_down, cv::Size(480, 270), cv::INTER_LINEAR);
                // cv::imshow("Test",resized_down);
                break;
            }

        }
        ImageBuffer buffer = ImageBuffer::getInstance();
        buffer.setFrame(resized_down, m_camera);
        buffer.show(m_camera);
    }
}
