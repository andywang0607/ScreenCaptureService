#ifndef __SCREENCAPTURE_H__
#define __SCREENCAPTURE_H__

#include "opencv2/opencv.hpp"
#include "screencapture_export.h"

class ScreenCapture
{
public:
    ScreenCapture() = default;
    ~ScreenCapture() = default;

    SCREENCAPTURE_EXPORT std::tuple<unsigned int, unsigned int> getCurrentScreenSize() const;

    SCREENCAPTURE_EXPORT cv::Mat captureScreenRect(unsigned int offsetX, unsigned int offsetY, unsigned int sizeX, unsigned int sizeY);
};
#endif // __SCREENCAPTURE_H__