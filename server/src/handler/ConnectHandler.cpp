#include "ConnectHandler.h"
#include "ServerConfig.hpp"

#include <spdlog/spdlog.h>

bool ConnectHandler::handle(MessageHelper &request, MessageHelper &response)
{
    updateAction(request, response);
    updateConnectToken(request, response);

    auto action = request.get("action");
    if (action == "connect") {
        auto &&[width, height] = screenCapture_.getCurrentScreenSize();

        std::string myIp = ServerConfig::getInstance().get("serverIp");
        std::string publishPort = std::to_string(ServerConfig::getInstance().get<int>("publishPort", -1));
        std::string publicAddress = "tcp://" + myIp + ":" + publishPort;

        spdlog::info("[ConnectHandler] publicAddress: {}", publicAddress);

        response.set("imgWidth", width);
        response.set("imgHeight", height);
        response.set("topic", "screenImage");
        response.set("publicAddress", publicAddress);

        spdlog::info("[ConnectHandler] response: {}", response.toString());
        return true;
    }

    spdlog::error("[ConnectHandler] unknown action: {}", action);
    return false;
}
