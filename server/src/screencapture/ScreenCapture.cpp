#include <atlimage.h>

#include "ScreenCapture.h"
#include "opencv2/opencv.hpp"

std::tuple<unsigned int, unsigned int> ScreenCapture::getCurrentScreenSize() const
{
    HDC hdc = GetDC(nullptr);
    RECT rc;
    GetClientRect(GetDesktopWindow(), &rc);

    return std::make_tuple(abs(rc.right), abs(rc.bottom));
}

std::vector<unsigned char> ScreenCapture::captureScreenRect(unsigned int offsetX, unsigned int offsetY, unsigned int sizeX, unsigned int sizeY)
{
    HDC hdc = GetDC(nullptr);
    RECT rc;
    GetClientRect(GetDesktopWindow(), &rc);
    int cx = rc.right;
    int cy = rc.bottom;

    CImage image;
    image.Create(cx, -cy, 32);

    BitBlt(image.GetDC(), offsetX, offsetY, sizeX, sizeY, hdc, 0, 0, SRCCOPY);
    image.ReleaseDC();
    ReleaseDC(0, hdc);

    static cv::Mat bgra(sizeY, sizeX, CV_8UC4);
    memcpy(bgra.data, image.GetBits(), sizeY * sizeX * 4);

    std::vector<unsigned char> rgbVec;
    auto success = cv::imencode(".jpg", bgra, rgbVec); 
    return rgbVec;
}
