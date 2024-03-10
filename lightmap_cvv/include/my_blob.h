//
// Created by valen on 3/10/2024.
//

#ifndef LIGHTMAP_CV_MY_BLOB_H
#define LIGHTMAP_CV_MY_BLOB_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class Blob{
public:
    std::vector<cv::Point> currentContour;

    cv::Rect currentBoundingRect;

    std::vector<cv::Point> centerPositions;

    double dblCurrentDiagonalSize;
    double dblCurrentAspectRatio;

    bool blnCurrentMatchFoundOrNewBlob;

    bool blnStillBeingTracked;

    int intNumOfConsecutiveFramesWithoutAMatch;

    cv::Point predictedNextPosition;


    Blob(std::vector<cv::Point> _contour);
    void predictNextPosition(void);
};

#endif //LIGHTMAP_CV_MY_BLOB_H
