#include "QueryScreenImageHandler.h"
#include "ScreenCapture.h"

#include <thread>
#include <vector>
#include <atomic>
#include <string>

#include <zmqpp/proxy.hpp>
#include <zmqpp/zmqpp.hpp>
#include <base64.h>

struct QueryScreenImageHandler::impl
{
    impl(zmqpp::context &context)
        : context_(context)
        , publishSocket_(context_, zmqpp::socket_type::publish)
        , isPublishStart_(false)
        , subscribeClientNum_(0)
    {
        std::string myIp = "192.168.2.88";
        std::string publishPort = "8081";
        std::string publicAddress = "tcp://" + myIp + ":" + publishPort;

        publishSocket_.set(zmqpp::socket_option::heartbeat_interval, 120000);
        publishSocket_.set(zmqpp::socket_option::heartbeat_timeout, 240000);
        publishSocket_.bind(publicAddress);
    }

    ~impl()
    {
        stopPublish();
    }

    void startPublish()
    {
        isPublishStart_.store(true, std::memory_order_release);
        publishThread_.emplace_back([this]() {
            auto &&[width, height] = screenCapture_.getCurrentScreenSize();

            MessageHelper publishResponse;
            publishResponse.set("imgWidth", width);
            publishResponse.set("imgHeight", height);
            publishResponse.set("action", "imageRtn");
            publishResponse.setTopic("screenImage");

            while (isPublishStart_) {
                auto imgData = screenCapture_.captureScreenRect(0, 0, width, height);
                auto imgDataStr = base64_encode(imgData.data(), imgData.size());

                publishResponse.set("imgData", imgDataStr);

                zmqpp::message response = publishResponse.toMessage();
                publishSocket_.send(response);
            }
        });
    }

    void stopPublish()
    {
        isPublishStart_.store(false, std::memory_order_release);
        for (auto &thread : publishThread_) {
            thread.join();
        }
        publishThread_.clear();
    }

    ScreenCapture screenCapture_;

    std::atomic_bool isPublishStart_;
    std::atomic_uint32_t subscribeClientNum_;
    std::vector<std::thread> publishThread_;

    zmqpp::context &context_;
    zmqpp::socket publishSocket_;
};

QueryScreenImageHandler::QueryScreenImageHandler(zmqpp::context &context) 
    : Handler()
    , pimpl_(std::make_unique<impl>(context))
{
    
}

bool QueryScreenImageHandler::handle(MessageHelper &request, MessageHelper &response)
{
    updateAction(request, response);
    updateConnectToken(request, response);

    auto action = request.get("action");
    if (action == "startQueryScreenImage") {
        pimpl_->subscribeClientNum_.fetch_add(1, std::memory_order_release);

        if (pimpl_->subscribeClientNum_.load(std::memory_order_acquire) > 1) {
            response.set("message", "publishThread already exist");
        } else {
            pimpl_->startPublish();
            response.set("message", "start publishThread success");
        }
        return true;
    }
    if (action == "stopQueryScreenImage") {
        pimpl_->subscribeClientNum_.fetch_sub(1, std::memory_order_release);

        if (pimpl_->subscribeClientNum_.load(std::memory_order_acquire) == 0) {
            pimpl_->stopPublish();
            response.set("message", "stopQueryScreenImage sucess, publish thread stop");
        } else {
            response.set("message", "stopQueryScreenImage sucess");
        }
        return true;
    }

    return false;
}
