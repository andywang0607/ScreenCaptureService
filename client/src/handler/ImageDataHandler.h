#ifndef __IMAGEDATAHANDLER_H__
#define __IMAGEDATAHANDLER_H__

#include "Handler.h"

class ImageDataHandler : public Handler
{
public:
    ImageDataHandler() = default;
    ~ImageDataHandler() override = default;

    bool handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl* api) override;
};

#endif // __IMAGEDATAHANDLER_H__