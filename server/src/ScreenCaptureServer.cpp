#include "ScreenCaptureServer.hpp"
#include "MessageHelper.hpp"
#include "ScreenCapture.h"

#include <atomic>
#include <iostream>
#include <vector>

#include <zmqpp/proxy.hpp>
#include <zmqpp/zmqpp.hpp>
#include <base64.h>

struct ScreenCaptureServer::impl
{
    impl()
        : serviceSocket_(context_, zmqpp::socket_type::router)
        , publishSocket_(context_, zmqpp::socket_type::publish)
        , isStart_(false)
        , isPublishStart_(false)
    {
    }

    ~impl()
    {
        stopService();
    }

    void stopService()
    {
        isStart_.store(false, std::memory_order_release);
        for (auto &thread : serviceThread_) {
            thread.join();
        }
        serviceThread_.clear();
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

    std::atomic_bool isStart_;
    std::atomic_bool isPublishStart_;
    std::atomic_uint32_t subscribeClientNum_;
    std::vector<std::thread> serviceThread_;
    std::vector<std::thread> publishThread_;

    zmqpp::context context_;
    zmqpp::socket serviceSocket_;
    zmqpp::socket publishSocket_;
};

ScreenCaptureServer::ScreenCaptureServer(): pimpl_(std::make_unique<impl>())
{
    std::string myIp = "192.168.2.88";
    std::string myPort = "8080";
    std::string publishPort = "8081";
    std::string serviceAddress = "tcp://" + myIp + ":" + myPort;
    std::string publicAddress = "tcp://" + myIp + ":" + publishPort;

    std::cout << "serviceAddress: " << serviceAddress << "\n";
    std::cout << "publicAddress: " << publicAddress << "\n";

    pimpl_->serviceSocket_.set(zmqpp::socket_option::heartbeat_interval, 120000);
    pimpl_->serviceSocket_.set(zmqpp::socket_option::heartbeat_timeout, 240000);
    pimpl_->serviceSocket_.bind(serviceAddress);

    pimpl_->publishSocket_.set(zmqpp::socket_option::heartbeat_interval, 120000);
    pimpl_->publishSocket_.set(zmqpp::socket_option::heartbeat_timeout, 240000);
    pimpl_->publishSocket_.bind(publicAddress);

    std::cout << "ScreenCaptureServer cunstruct success \n";
}

ScreenCaptureServer::~ScreenCaptureServer()
{
}

void ScreenCaptureServer::startService()
{
    pimpl_->isStart_.store(true, std::memory_order_release);
    pimpl_->serviceThread_.emplace_back([&]() {
        std::cout << "start service Thread \n";
        while (pimpl_->isStart_.load(std::memory_order_acquire)) {
            zmqpp::message request;
            pimpl_->serviceSocket_.receive(request);

            MessageHelper msgRequest(request);
            std::cout << "request to string: " << msgRequest.toString() << "\n";

            auto action = msgRequest.get("action");
            MessageHelper msgResponse;
            msgResponse.setConnectToken(msgRequest.getConnectToken());
            msgResponse.set("action", action);

            if (action == "startQueryScreenImage") {
                pimpl_->subscribeClientNum_.fetch_add(1, std::memory_order_release);
                if (pimpl_->subscribeClientNum_.load(std::memory_order_acquire) > 1) {
                    msgResponse.set("message", "publishThread already exist");
                } else {
                    pimpl_->isPublishStart_.store(true, std::memory_order_release);
                    pimpl_->publishThread_.emplace_back([this]() {
                        auto &&[width, height] = pimpl_->screenCapture_.getCurrentScreenSize();

                        MessageHelper publishResponse;
                        publishResponse.set("imgWidth", width);
                        publishResponse.set("imgHeight", height);
                        publishResponse.set("action", "imageRtn");
                        publishResponse.setTopic("screenImage");

                        while (pimpl_->isPublishStart_) {
                            auto imgData = pimpl_->screenCapture_.captureScreenRect(0, 0, width, height);
                            auto imgDataStr = base64_encode(imgData.data(), imgData.size());

                            publishResponse.set("imgData", imgDataStr);

                            zmqpp::message response = publishResponse.toMessage();
                            pimpl_->publishSocket_.send(response);
                        }
                    });
                    msgResponse.set("message", "start publishThread success");
                }
               
            } else if (action == "connect") {
                auto &&[width, height] = pimpl_->screenCapture_.getCurrentScreenSize();

                msgResponse.set("imgWidth", width);
                msgResponse.set("imgHeight", height);
                msgResponse.set("topic", "screenImage");
            } else if (action == "stopQueryScreenImage") {
                pimpl_->subscribeClientNum_.fetch_sub(1, std::memory_order_release);
                if(pimpl_->subscribeClientNum_.load(std::memory_order_acquire) == 0) {
                    pimpl_->stopPublish();
                    msgResponse.set("message", "stopQueryScreenImage sucess, publish thread stop");
                } else {
                    msgResponse.set("message", "stopQueryScreenImage sucess");
                }
            }

            zmqpp::message response = msgResponse.toMessage();
            pimpl_->serviceSocket_.send(response);
        }
        std::cout << "end service Thread \n";
    });
}

bool ScreenCaptureServer::getIsStart() 
{
    return pimpl_->isStart_.load(std::memory_order_acquire);
}
