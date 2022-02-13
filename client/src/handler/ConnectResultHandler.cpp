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
        auto publicAddress = receive.get("publicAddress");

        api->setTopic(topic);
        api->connectSubscribeSocket(publicAddress);
        spi->onConnectRspRtn(width, height);

        return true;
    }

    return false;
}
