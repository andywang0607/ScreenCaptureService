#ifndef __SCREENCAPTURE_H__
#define __SCREENCAPTURE_H__

#include <vector>
#include <tuple>

class ScreenCapture
{
public:
    ScreenCapture() = default;
    ~ScreenCapture() = default;

    std::tuple<unsigned int, unsigned int> getCurrentScreenSize() const;

    std::vector<unsigned char> captureScreenRect(unsigned int offsetX, unsigned int offsetY, unsigned int sizeX, unsigned int sizeY);
};
#endif // __SCREENCAPTURE_H__