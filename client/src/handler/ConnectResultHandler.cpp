#include "ConnectResultHandler.h"
#include "ScreenCaptureApiImpl.h"
#include "MessageHelper.hpp"

bool ConnectResultHandler::handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl *api)
{
    auto action = receive.get("action");
    
    if (action == "connect") {
        auto width = receive.get<int>("imgWidth", -1);
        auto height = receive.get<int>("imgHeight", -1);
        auto topic = receive.get("topic");
        api->setTopic(topic);

        api->connectSubscribeSocket("tcp://192.168.2.88:8081");
        spi->onConnectRspRtn(width, height);

        return true;
    }

    return false;
}
