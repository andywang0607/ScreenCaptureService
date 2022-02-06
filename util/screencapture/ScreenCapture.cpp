#include <atlimage.h>
#include <tuple>

#include "ScreenCapture.h"

std::tuple<unsigned int, unsigned int> ScreenCapture::getCurrentScreenSize() const
{
    HDC hdc = GetDC(nullptr);
    RECT rc;
    GetClientRect(GetDesktopWindow(), &rc);

    return std::make_tuple(abs(rc.right), abs(rc.bottom));
}

cv::Mat ScreenCapture::captureScreenRect(unsigned int offsetX, unsigned int offsetY, unsigned int sizeX, unsigned int sizeY)
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

    cv::Mat mat;
    mat.create(sizeY, sizeX, CV_8UC4);
    memcpy(mat.data, image.GetBits(), sizeY * sizeX * 4);
    
    cv::Mat ret; 
    cv::cvtColor(mat, ret, cv::COLOR_BGRA2BGR);
    return ret;
}
