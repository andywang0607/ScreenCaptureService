#ifndef __SCREENCAPTUREAPIIMPL_H__
#define __SCREENCAPTUREAPIIMPL_H__

#include "MessageHelper.hpp"
#include "include/ScreenCaptureApi.h"

#include <vector>
#include <thread>
#include <atomic>
#include <queue>
#include <string>

#include <nlohmann/json.hpp>
#include <zmqpp/zmqpp.hpp>
#include <zmqpp/reactor.hpp>

class ScreenCaptureApiImpl : public ScreenCaptureApi
{
public:
    ScreenCaptureApiImpl(ScreenCaptureSpi &spi);
    ~ScreenCaptureApiImpl() override;

    int connect(const char *ip, int port) override;
    int startQueryScreenImage() override;
    int stopQueryScreenImage() override;
    void disconnect() override;

    void connectSubscribeSocket(const std::string &address);
    void subscribeSocket(const std::string &topic);
    void unsubSubscribeSocket(const std::string &topic);
    
    void setTopic(const std::string &topic);
private:
    void handleMessage(zmqpp::socket &socket);
    void addSendQueue(nlohmann::json &msg);
    std::string genConnectToken();

    void start();
    void stop();

    zmqpp::context context_;
    zmqpp::socket requestSocket_;
    zmqpp::socket subscribeSocket_;
    zmqpp::reactor reactor_;

    std::atomic_bool isStart_;
    std::vector<std::thread> threads_;
    std::string topic_;
    std::queue<nlohmann::json> sendQueue_;
    ScreenCaptureSpi &spi_;
};

#endif // __SCREENCAPTUREAPIIMPL_H__