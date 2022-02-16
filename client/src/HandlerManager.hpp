#ifndef __HANDLERMANAGER_H__
#define __HANDLERMANAGER_H__

#include "handler/ConnectResultHandler.h"
#include "handler/QueryScreenImageResultHandler.h"
#include "handler/SubscriptionHandler.h"

#include <memory>
#include <unordered_map>

#include <spdlog/spdlog.h>

class HandlerManager
{
public:
    static bool handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl *api)
    {
        auto action = receive.get("action");

        if (action == "connect") {
            return std::make_unique<ConnectResultHandler>()->handle(receive, spi, api);
        } else if (action == "startQueryScreenImage") {
            return std::make_unique<QueryScreenImageResultHandler>()->handle(receive, spi, api);
        } else if (action == "stopQueryScreenImage") {
            return std::make_unique<QueryScreenImageResultHandler>()->handle(receive, spi, api);
        } else if (action == "imageRtn") {
            return std::make_unique<SubscriptionHandler>()->handle(receive, spi, api);
        }

        spdlog::error("[HandlerManager] unknown action: {}", action);
        return false;
    }
};

#endif // __HANDLERMANAGER_H__