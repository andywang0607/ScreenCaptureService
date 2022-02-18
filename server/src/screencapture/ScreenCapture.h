#ifndef __SCREENCAPTURE_H__
#define __SCREENCAPTURE_H__

#include <atlimage.h>

#include <vector>
#include <tuple>

#include "opencv2/opencv.hpp"

class ScreenCapture
{
public:
    ScreenCapture()
    {
        hdc_ = GetDC(nullptr);
        GetClientRect(GetDesktopWindow(), &rc_);
        screenWidth_ = abs(rc_.right);
        screenHeight_ = abs(rc_.bottom);

        image_.Create(rc_.right, -rc_.bottom, 32);

        bgra_ = cv::Mat(screenHeight_, screenWidth_, CV_8UC4);
        rgbVec_.reserve(bgra_.total() * 1024);
    }

    ~ScreenCapture()
    {
        image_.ReleaseDC();
        ReleaseDC(0, hdc_);
    }

    std::tuple<unsigned int, unsigned int> getCurrentScreenSize() const;

    std::vector<unsigned char> captureScreenRect(unsigned int offsetX, unsigned int offsetY, unsigned int sizeX, unsigned int sizeY);

private:
    HDC hdc_;
    RECT rc_;

    int screenWidth_;
    int screenHeight_;

    CImage image_;

    cv::Mat bgra_;
    std::vector<unsigned char> rgbVec_;
};
#endif // __SCREENCAPTURE_H__