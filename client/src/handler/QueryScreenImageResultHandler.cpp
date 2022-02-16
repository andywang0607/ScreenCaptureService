#include "QueryScreenImageResultHandler.h"
#include "MessageHelper.hpp"
#include "ScreenCaptureApiImpl.h"

bool QueryScreenImageResultHandler::handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl *api)
{
    auto action = receive.get("action");

    if (action == "startQueryScreenImage") {
        spi->onStartQueryScreenImageRspRtn(receive.get("message").c_str());

        return true;
    }
    if (action == "stopQueryScreenImage") {
        spi->onStopQueryScreenImageRspRtn(receive.get("message").c_str());

        return true;
    }

    return false;
}
