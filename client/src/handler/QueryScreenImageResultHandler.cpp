#include "QueryScreenImageResultHandler.h"
#include "MessageHelper.hpp"
#include "ScreenCaptureApiImpl.h"

bool QueryScreenImageResultHandler::handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl *api)
{
    auto action = receive.get("action");

    if (action == "startQueryScreenStream") {
        spi->onStartQueryScreenStreamRspRtn(receive.get("message").c_str());

        return true;
    }
    if (action == "stopQueryScreenStream") {
        spi->onStopQueryScreenStreamRspRtn(receive.get("message").c_str());

        return true;
    }

    return false;
}
