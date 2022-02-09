#include "ScreenCaptureServer.hpp"
#include "MessageHelper.hpp"

#include <atomic>
#include <iostream>
#include <vector>

#include <zmqpp/proxy.hpp>
#include <zmqpp/zmqpp.hpp>

struct ScreenCaptureServer::impl
{
    impl()
        : serviceSocket_(context_, zmqpp::socket_type::router)
        , isStart_(false)
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

    std::atomic_bool isStart_;
    std::vector<std::thread> serviceThread_;

    zmqpp::context context_;
    zmqpp::socket serviceSocket_;
};

ScreenCaptureServer::ScreenCaptureServer(): pimpl_(std::make_unique<impl>())
{
    std::string myIp = "192.168.2.88";
    std::string myPort = "8080";
    std::string serviceAddress = "tcp://" + myIp + ":" + myPort;

    std::cout << "serviceAddress: " << serviceAddress << "\n";

    pimpl_->serviceSocket_.set(zmqpp::socket_option::heartbeat_interval, 120000);
    pimpl_->serviceSocket_.set(zmqpp::socket_option::heartbeat_timeout, 240000);
    pimpl_->serviceSocket_.bind(serviceAddress);

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

            MessageHelper msgResponse;
            msgResponse.setConnectToken(msgRequest.getConnectToken());
            msgResponse.set("test", 123);

            zmqpp::message response = msgResponse.toMessage();
            pimpl_->serviceSocket_.send(response);
            std::cout << " reply message: " << msgResponse.toString() << "\n";
        }
        std::cout << "end service Thread \n";
    });
}

bool ScreenCaptureServer::getIsStart() 
{
    return pimpl_->isStart_.load(std::memory_order_acquire);
}
