#include "ScreenCaptureApi.h"

#include <iostream>

#include <zmqpp/message.hpp>
#include <zmqpp/reactor.hpp>
#include <zmqpp/socket_options.hpp>
#include <zmqpp/socket_types.hpp>

ScreenCaptureApi::ScreenCaptureApi()
    : requestSocket_(context_, zmqpp::socket_type::dealer)
    , isStart_(true)
{
    reactor_.add(requestSocket_, [this]() {
        std::cout << "reactor lambda \n";
        handleMessage(requestSocket_);
    });

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
    std::cout << "ScreenCaptureServer cunstruct success \n";
}

ScreenCaptureApi::~ScreenCaptureApi()
{
    isStart_.store(false);
    for (auto &thread : threads_) {
        thread.join();
    }
    threads_.clear();
}

int ScreenCaptureApi::connect()
{
    std::string token = "hafjkdasfblasjkfbasjlhfgjkasdfbal";
    requestSocket_.set(zmqpp::socket_option::identity, token);
    requestSocket_.set(zmqpp::socket_option::heartbeat_interval, 120000);
    requestSocket_.set(zmqpp::socket_option::heartbeat_timeout, 240000);

    std::string requestEndpoint = "tcp://192.168.2.88:8080";
    requestSocket_.connect(requestEndpoint);

    MessageHelper request;
    request.set("IP", "127.0.0.1");
    addSendQueue(request.getBody());

    std::cout << " request message: " << request.toString() << "\n";
}

void ScreenCaptureApi::handleMessage(zmqpp::socket &socket)
{
    zmqpp::message receive;
    socket.receive(receive);

    MessageHelper msgReceive(receive);
    std::cout << "msgReceive: " << msgReceive.toString() << "\n";
}

void ScreenCaptureApi::addSendQueue(nlohmann::json &msg)
{
    sendQueue_.push(msg);
}