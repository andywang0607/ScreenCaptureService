#include "ScreenCaptureApiImpl.h"
#include "HandlerManager.hpp"

#include <chrono>
#include <string>

#include <base64.h>
#include <md5.h>
#include <zmqpp/message.hpp>
#include <zmqpp/reactor.hpp>
#include <zmqpp/socket_options.hpp>
#include <zmqpp/socket_types.hpp>
#include <spdlog/spdlog.h>

ScreenCaptureApi *ScreenCaptureApi::create(ScreenCaptureSpi &spi)
{
    return new ScreenCaptureApiImpl(spi);
}

ScreenCaptureApiImpl::ScreenCaptureApiImpl(ScreenCaptureSpi &spi)
    : requestSocket_(context_, zmqpp::socket_type::dealer)
    , subscribeSocket_(context_, zmqpp::socket_type::subscribe)
    , isStart_(true)
    , spi_(spi)
{
    reactor_.add(requestSocket_, [&, this]() {
        handleMessage(requestSocket_);
    });

    reactor_.add(subscribeSocket_, [this]() {
        handleMessage(subscribeSocket_);
    });

}

ScreenCaptureApiImpl::~ScreenCaptureApiImpl()
{
    stop();
}

int ScreenCaptureApiImpl::connect(const char *ip, int port)
{
    start();

    std::string token = genConnectToken();
    requestSocket_.set(zmqpp::socket_option::identity, token);
    requestSocket_.set(zmqpp::socket_option::heartbeat_interval, 120000);
    requestSocket_.set(zmqpp::socket_option::heartbeat_timeout, 240000);

    std::string requestEndpoint = "tcp://" + std::string(ip) + ":" + std::to_string(port);
    requestSocket_.connect(requestEndpoint);

    MessageHelper request;
    request.set("action", "connect");
    request.set("ip", std::string(ip));
    request.set("port", std::to_string(port));
    addSendQueue(request.getBody());

    spdlog::info("[ScreenCaptureApiImpl] request message: {}", request.toString());
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

    spdlog::info("[ScreenCaptureApiImpl] request message: {}", request.toString());
    return 0;
}

int ScreenCaptureApiImpl::stopQueryScreenImage()
{
    unsubSubscribeSocket(topic_);

    MessageHelper request;

    request.set("action", "stopQueryScreenImage");
    addSendQueue(request.getBody());

    spdlog::info("[ScreenCaptureApiImpl] request message: {}", request.toString());
    return 0;
}

void ScreenCaptureApiImpl::disconnect()
{
    MessageHelper request;

    request.set("action", "disconnect");
    addSendQueue(request.getBody());

    spdlog::info("[ScreenCaptureApiImpl] request message: {}", request.toString());

    stop();
    requestSocket_.close();
    subscribeSocket_.close();
    context_.terminate();

    spi_.onDisConnectRspRtn("disconnect success");
}

void ScreenCaptureApiImpl::setTopic(const std::string &topic)
{
    topic_ = topic;
}

void ScreenCaptureApiImpl::handleMessage(zmqpp::socket &socket)
{
    zmqpp::message receive;
    socket.receive(receive);

    MessageHelper msgReceive(receive);
    HandlerManager::handle(msgReceive, &spi_, this);
}

void ScreenCaptureApiImpl::addSendQueue(nlohmann::json &msg)
{
    sendQueue_.push(msg);
}

std::string ScreenCaptureApiImpl::genConnectToken()
{
    using namespace std::chrono;
    std::string currentTime = std::to_string(duration_cast<microseconds>(system_clock::now().time_since_epoch()).count());

    return md5(currentTime);
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
        spdlog::info("[ScreenCaptureApiImpl] polling thread start");
        while (isStart_.load(std::memory_order_acquire)) {
            while (!sendQueue_.empty()) {
                auto request = sendQueue_.try_pop();
                if (!request) {
                    continue;
                }
                requestSocket_.send((*request).dump());
            }
            reactor_.poll(100);
        }
        spdlog::info("[ScreenCaptureApiImpl] polling thread finish");
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