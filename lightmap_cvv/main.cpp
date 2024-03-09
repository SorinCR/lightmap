#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "include/camera_provider.h"

using namespace cv;
using namespace std;

int t = 0;

int main() {
    try
    {
        std::cout << "running" << std::endl;
        CameraProvider::run();
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception catch: " << e.what() << "\n";
    }
    return 0;
}