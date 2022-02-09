#ifndef __SCREENCAPTUREAPI_H__
#define __SCREENCAPTUREAPI_H__

#include "MessageHelper.hpp"

#include <vector>
#include <thread>
#include <atomic>
#include <queue>

#include <nlohmann/json.hpp>
#include <zmqpp/zmqpp.hpp>
#include <zmqpp/reactor.hpp>

class ScreenCaptureApi
{
public:
    ScreenCaptureApi();
    ~ScreenCaptureApi();

    int connect();

private:
    void handleMessage(zmqpp::socket &socket);
    void addSendQueue(nlohmann::json &msg);

    zmqpp::context context_;
    zmqpp::socket requestSocket_;
    zmqpp::reactor reactor_;

    std::atomic_bool isStart_;
    std::vector<std::thread> threads_;
    std::queue<nlohmann::json> sendQueue_;
};

#endif // __SCREENCAPTUREAPI_H__