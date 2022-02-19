#include "ImageDataHandler.h"
#include "ScreenCaptureApiImpl.h"
#include "MessageHelper.hpp"

#include <base64.h>

bool ImageDataHandler::handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl* api) 
{
    auto action = receive.get("action");

    if (action == "streamRtn") {
        auto imgDataReceive = receive.get("imgData");
        auto imgData = base64_decode(imgDataReceive);

        spi->onStreamRtn((unsigned char *)imgData.c_str(), imgData.size());

        return true;
    }
    if (action == "imageRtn") {
        auto imgDataReceive = receive.get("imgData");
        auto imgData = base64_decode(imgDataReceive);

        spi->onImageRtn((unsigned char *)imgData.c_str(), imgData.size());

        return true;
    }

    return false;
}
