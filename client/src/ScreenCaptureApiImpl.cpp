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
    , subscribeSocket_(context_, zmqpp::socket_type::subscribe)
    , isStart_(true)
    , spi_(spi)
{
    reactor_.add(requestSocket_, [&,this]() {
        handleMessage(requestSocket_);
    });

    reactor_.add(subscribeSocket_, [this]() {
        handleMessage(subscribeSocket_);
    });
    
    std::cout << "ScreenCaptureServer cunstruct success \n";
}

ScreenCaptureApiImpl::~ScreenCaptureApiImpl()
{
    stop();
}

int ScreenCaptureApiImpl::connect(const char *ip, int port)
{
    start();
    
    std::string token = "hafjkdasfblasjkfbasjlhfgjkasdfbal";
    requestSocket_.set(zmqpp::socket_option::identity, token);
    requestSocket_.set(zmqpp::socket_option::heartbeat_interval, 120000);
    requestSocket_.set(zmqpp::socket_option::heartbeat_timeout, 240000);

    std::string requestEndpoint = "tcp://" + std::string(ip) + ":" + std::to_string(port);
    requestSocket_.connect(requestEndpoint);

    MessageHelper request;
    request.set("action", "connect");
    request.set("IP", "127.0.0.1");
    addSendQueue(request.getBody());

    std::cout << " request message: " << request.toString() << "\n";

    return 0;
}

int ScreenCaptureApiImpl::startQueryScreenImage() 
{
    subscribeSocket(topic_);
    
    MessageHelper request;
    
    request.set("action", "startQueryScreenImage");
    request.set("width", 1900);
    request.set("height", 1080);
    addSendQueue(request.getBody());

    std::cout << " request message: " << request.toString() << "\n";
    return 0;
}

int ScreenCaptureApiImpl::stopQueryScreenImage() 
{
    unsubSubscribeSocket(topic_);

    MessageHelper request;
    
    request.set("action", "stopQueryScreenImage");
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
    subscribeSocket_.close();
    context_.terminate();

    spi_.onDisConnectRspRtn("disconnect success");
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
        auto topic = msgReceive.get("topic");
        topic_ = topic;

        connectSubscribeSocket("tcp://192.168.2.88:8081");
        spi_.onConnectRspRtn(width, height);
    } else if(action == "startQueryScreenImage") {
        spi_.onStartQueryScreenImageRspRtn(msgReceive.get("message").c_str());
    } else if(action == "stopQueryScreenImage") {
        spi_.onStopQueryScreenImageRspRtn(msgReceive.get("message").c_str());
    } else if(action == "imageRtn") {
        auto imgDataReceive = msgReceive.get("imgData");
        auto imgData = base64_decode(imgDataReceive);

        spi_.onImageRtn((unsigned char *)imgData.c_str(), imgData.size());
    }

    std::cout << "handleMessage finish \n";
}

void ScreenCaptureApiImpl::addSendQueue(nlohmann::json &msg)
{
    sendQueue_.push(msg);
}

void ScreenCaptureApiImpl::connectSubscribeSocket(const std::string &address) 
{
    subscribeSocket_.connect(address);
}

void ScreenCaptureApiImpl::subscribeSocket(const std::string &topic) 
{
    subscribeSocket_.subscribe(topic);
}

void ScreenCaptureApiImpl::unsubSubscribeSocket(const std::string &topic) 
{
    subscribeSocket_.unsubscribe(topic);
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