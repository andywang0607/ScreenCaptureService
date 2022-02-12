#include "ScreenCaptureApiImpl.h"

#include <iostream>

#include <zmqpp/message.hpp>
#include <zmqpp/reactor.hpp>
#include <zmqpp/socket_options.hpp>
#include <zmqpp/socket_types.hpp>
#include <base64.h>


ScreenCaptureApi* ScreenCaptureApi::create(ScreenCaptureSpi &spi)
{
    return new ScreenCaptureApiImpl(spi);
}

ScreenCaptureApiImpl::ScreenCaptureApiImpl(ScreenCaptureSpi &spi)
    : requestSocket_(context_, zmqpp::socket_type::dealer)
    , isStart_(true)
    , spi_(spi)
{
    reactor_.add(requestSocket_, [&,this]() {
        std::cout << "reactor lambda \n";
        handleMessage(requestSocket_);
    });
    
    std::cout << "ScreenCaptureServer cunstruct success \n";
}

ScreenCaptureApiImpl::~ScreenCaptureApiImpl()
{
    stop();
}

int ScreenCaptureApiImpl::connect()
{
    start();
    
    std::string token = "hafjkdasfblasjkfbasjlhfgjkasdfbal";
    requestSocket_.set(zmqpp::socket_option::identity, token);
    requestSocket_.set(zmqpp::socket_option::heartbeat_interval, 120000);
    requestSocket_.set(zmqpp::socket_option::heartbeat_timeout, 240000);

    std::string requestEndpoint = "tcp://192.168.2.88:8080";
    requestSocket_.connect(requestEndpoint);

    MessageHelper request;
    request.set("action", "connect");
    request.set("IP", "127.0.0.1");
    addSendQueue(request.getBody());

    std::cout << " request message: " << request.toString() << "\n";

    return 0;
}

int ScreenCaptureApiImpl::queryScreenImage() 
{
    MessageHelper request;
    
    request.set("action", "queryScreenImage");
    request.set("width", 1900);
    request.set("height", 1080);
    addSendQueue(request.getBody());

    std::cout << " request message: " << request.toString() << "\n";
    return 0;
}

void ScreenCaptureApiImpl::disconnect() 
{
    MessageHelper request;
    
    request.set("action", "disconnect");
    addSendQueue(request.getBody());

    std::cout << " request message: " << request.toString() << "\n";

    stop();
    requestSocket_.close();
    context_.terminate();
}

void ScreenCaptureApiImpl::handleMessage(zmqpp::socket &socket)
{
    zmqpp::message receive;
    socket.receive(receive);

    MessageHelper msgReceive(receive);
    
    auto action = msgReceive.get("action");
    std::cout << "action: " << action << "\n";

    if(action == "connect") {
        auto width = msgReceive.get<int>("imgWidth", -1);
        auto height = msgReceive.get<int>("imgHeight", -1);

        spi_.onConnectReturn(width, height);
    } else if(action == "disconnect") {
        spi_.onDisConnectReturn("disconnect success");
    } else if(action == "queryScreenImage") {
        auto imgDataReceive = msgReceive.get("imgData");
        auto imgData = base64_decode(imgDataReceive);

        spi_.onImgReturn((unsigned char *)imgData.c_str(), imgData.size());
    }

    std::cout << "handleMessage finish \n";
}

void ScreenCaptureApiImpl::addSendQueue(nlohmann::json &msg)
{
    sendQueue_.push(msg);
}

void ScreenCaptureApiImpl::start()
{
    threads_.emplace_back([&]() {
        std::cout << "poll thread start \n";
        while (isStart_.load(std::memory_order_acquire)) {
            while (!sendQueue_.empty()) {
                auto request = sendQueue_.front();
                requestSocket_.send(request.dump());
                sendQueue_.pop();
            }
            reactor_.poll(100);
        }
        std::cout << "poll thread end \n";
    });
}

void ScreenCaptureApiImpl::stop() 
{
    isStart_.store(false);
    for (auto &thread : threads_) {
        thread.join();
    }
    threads_.clear();
}