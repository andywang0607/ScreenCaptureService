#ifndef __SCREENCAPTUREAPIIMPL_H__
#define __SCREENCAPTUREAPIIMPL_H__

#include "MessageHelper.hpp"
#include "include/ScreenCaptureApi.h"
#include "threadsafe_queue.hpp"

#include <vector>
#include <thread>
#include <atomic>
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
    int queryScreenImage(int targetWidth = -1, int targetHeight = -1) override;
    int startQueryScreenStream(int targetWidth = -1, int targetHeight = -1) override;
    int stopQueryScreenStream() override;
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
    threadsafe_queue<nlohmann::json> sendQueue_;
    ScreenCaptureSpi &spi_;
};

#endif // __SCREENCAPTUREAPIIMPL_H__