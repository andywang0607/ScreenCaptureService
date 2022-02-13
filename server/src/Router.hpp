#ifndef __ROUTER_HPP__
#define __ROUTER_HPP__

#include "handler/ConnectHandler.h"
#include "handler/QueryScreenImageHandler.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace zmqpp {
class context;
}

class Router
{
public:
    static void createRouteMap(zmqpp::context &context)
    {
        auto connectHandler = std::make_shared<ConnectHandler>();
        auto queryImageHandler = std::make_shared<QueryScreenImageHandler>(context);

        routeMap.emplace("connect", connectHandler);
        routeMap.emplace("startQueryScreenImage", queryImageHandler);
        routeMap.emplace("stopQueryScreenImage", queryImageHandler);
    }

    static bool handle(MessageHelper &request, MessageHelper &response)
    {
        auto &iter = routeMap.find(request.get("action"));
        if (iter == routeMap.end()) {
            return false;
        }

        return iter->second->handle(request, response);
    }

private:
    static inline std::unordered_map<std::string, std::shared_ptr<Handler>> routeMap;
};

#endif // __ROUTER_HPP__