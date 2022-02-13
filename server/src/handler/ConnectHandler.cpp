#include "ConnectHandler.h"

bool ConnectHandler::handle(MessageHelper &request, MessageHelper &response)
{
    updateAction(request, response);
    updateConnectToken(request, response);

    auto action = request.get("action");
    if (action == "connect") {
        auto &&[width, height] = screenCapture_.getCurrentScreenSize();

        response.set("imgWidth", width);
        response.set("imgHeight", height);
        response.set("topic", "screenImage");
    }

    return false;
}
