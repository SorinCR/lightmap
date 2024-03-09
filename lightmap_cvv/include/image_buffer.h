//
// Created by valen on 3/9/2024.
//
#include <array>
#include <cassert>
#include <memory>
#ifndef LIGHTMAP_CV_IMAGE_BUFFER_H
#define LIGHTMAP_CV_IMAGE_BUFFER_H
class ImageBuffer final
{
private:
    ImageBuffer(std::string name):
            m_name(name),
            m_currentColor(cv::Scalar(255,255,255)), // Default is white
            m_circleRadius(1),
            m_lineThickness(1),
            m_textFontScale(0.8F)
    {
        m_sketchedImageData = new uint8_t[(270*2)*(480*2) * 3];
        // cv::namedWindow(m_name, cv::WINDOW_AUTOSIZE);
    }

    /* Enum describing position in the 2x4 image. */
    enum ImagePositionX
    {
        ImagePositionLeft    = 0,
        ImagePositionRight   = 1,
        ImagePositionFarRight = 2
    };
    enum ImagePositionY
    {
        ImagePositionTop    = 0,
        ImagePositionBottom   = 1
    };

public:

    static ImageBuffer& getInstance(){
        static ImageBuffer instance("ELSED VISU");
        return instance;
    }


    void setFrame(cv::Mat& cvImage, int side)
    {
        ImagePositionX posX;
        ImagePositionY posY;
        switch (side)
        {
            case 0:
                posX = ImagePositionX::ImagePositionLeft;
                posY = ImagePositionY::ImagePositionTop;
                break;
            case 1:
                posX = ImagePositionX::ImagePositionRight;
                posY = ImagePositionY::ImagePositionTop;
                break;
            case 2:
                posX = ImagePositionX::ImagePositionLeft;
                posY = ImagePositionY::ImagePositionBottom;
                break;
            case 3:
                posX = ImagePositionX::ImagePositionRight;
                posY = ImagePositionY::ImagePositionBottom;
                break;
            case 4:
                posX = ImagePositionX::ImagePositionFarRight;
                posY = ImagePositionY::ImagePositionTop;
                break;
        }

        uint8_t const * srcDataPtr = cvImage.data;

        uint32_t const stride = (4 * static_cast<uint32_t>(cvImage.cols));
        std::cout<<cvImage.cols<<std::endl;
        uint32_t const dstStartRGBIndex = ((static_cast<uint32_t>(posY) * (stride * cvImage.rows))
                                           +  (static_cast<uint32_t>(posX) * (cvImage.cols)));
        uint32_t dstArrayIndex = dstStartRGBIndex * 3; // Follow array sizes

        m_sketchedImageData = getImageBuffer();
        //std::cout<<m_sketchedImageData;
        for (int pixelY = 0; pixelY < cvImage.rows; ++pixelY)
        {
            //std::cout<<pixelY<<std::endl;
            int const pixelLine = pixelY * cvImage.cols;
            for (int pixelX = 0; pixelX < cvImage.cols; ++pixelX)
            {
                int const inputIndexValue = (pixelLine + pixelX) * 3;
                m_sketchedImageData[dstArrayIndex + 0] = srcDataPtr[inputIndexValue + 0];
                m_sketchedImageData[dstArrayIndex + 1] = srcDataPtr[inputIndexValue + 1];
                m_sketchedImageData[dstArrayIndex + 2] = srcDataPtr[inputIndexValue + 2];

                dstArrayIndex += 3; // RGB has 3 elements in destination array
            }
            dstArrayIndex += (3 * cvImage.cols);
        }
    }

    void show(int camera_index)
    {
        int posX;
        int posY;
        std::string camera;
        switch (camera_index)
        {
            case 0:
                posX = 256;
                posY = 20;
                camera = "TopLeft";
                break;
            case 1:
                posX = 256 + 480;
                posY = 20;
                camera = "TopRight";
                break;
            case 2:
                posX = 256;
                posY = 20 + 270;
                camera = "BottomLeft";
                break;
            case 3:
                posX = 256 + 480;
                posY = 20 + 270;
                camera = "BottomRight";
                break;
            case 4:
                posX = 256 + 480 * 2;
                posY = 20;
                camera = "FarRight";
                break;
        }
        cv::Point2f textPosition(posX, posY);
        m_openCvImage = cv::Mat(270 * 2, 480 * 4, CV_8UC3, m_sketchedImageData);
        cv::putText(m_openCvImage,
                    camera.c_str(),
                    textPosition,
                    cv::FONT_HERSHEY_COMPLEX_SMALL,
                    m_textFontScale,
                    m_currentColor,
                    m_lineThickness);
        for(int x = 270;x<280;x++)
            for(int y=0;y<10;y++)
                m_openCvImage.at<cv::Vec3b>(x,y)=cv::Vec3b(0,0,255);
        for(int x = 0;x<10;x++)
            for(int y=960;y<970;y++)
                m_openCvImage.at<cv::Vec3b>(x,y)=cv::Vec3b(0,0,255);
        cv::imshow(m_name, m_openCvImage);
        cv::waitKey(2);
    }

private:
    std::string m_name;
    cv::Scalar  m_currentColor;       /**< Color used by drawing functions */
    int         m_circleRadius;       /**< Radius of drawn circles */
    int         m_lineThickness;      /**< Thickness of drawn line */
    float       m_textFontScale;      /**< Scale of the font used when drawing text */
    cv::Mat  m_openCvImage;
    uint8_t *m_sketchedImageData;

    static uint8_t* getImageBuffer()
    {
        static uint8_t *imagesPtr = new uint8_t[(480 * 270 * 3) * 8];
        return imagesPtr;
    }

};
#endif //LIGHTMAP_CV_IMAGE_BUFFER_H
