#include "SubscriptionHandler.h"
#include "ScreenCaptureApiImpl.h"
#include "MessageHelper.hpp"

#include <base64.h>

bool SubscriptionHandler::handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl* api) 
{
    auto action = receive.get("action");

    if (action == "imageRtn") {
        auto imgDataReceive = receive.get("imgData");
        auto imgData = base64_decode(imgDataReceive);

        spi->onImageRtn((unsigned char *)imgData.c_str(), imgData.size());

        return true;
    }

    return false;
}
