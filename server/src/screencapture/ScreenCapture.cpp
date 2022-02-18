#include "ScreenCapture.h"

#include <chrono>

#include <spdlog/spdlog.h>

std::tuple<unsigned int, unsigned int> ScreenCapture::getCurrentScreenSize() const
{
    return std::make_tuple(screenWidth_, screenHeight_);
}

std::vector<unsigned char> ScreenCapture::captureScreenRect(unsigned int offsetX, unsigned int offsetY, unsigned int sizeX, unsigned int sizeY)
{
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    
    BitBlt(image_.GetDC(), offsetX, offsetY, screenWidth_, screenHeight_, hdc_, 0, 0, SRCCOPY);
    
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

    bgra_.data = (uchar *)image_.GetBits();
    auto success = cv::imencode(".jpg", bgra_, rgbVec_);
    
    std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();

    spdlog::debug("[ScreenCapture] get screen image time: {} (ms)",
                 std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
    spdlog::debug("[ScreenCapture] cv::imencode time: {} (ms)",
                 std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count());
   if(success) {
      return rgbVec_; 
   }
   return {};
}
