#ifndef __SCREENCAPTUREAPIIMPL_H__
#define __SCREENCAPTUREAPIIMPL_H__

#include "MessageHelper.hpp"
#include "include/ScreenCaptureApi.h"

#include <vector>
#include <thread>
#include <atomic>
#include <queue>

#include <nlohmann/json.hpp>
#include <zmqpp/zmqpp.hpp>
#include <zmqpp/reactor.hpp>

class ScreenCaptureApiImpl : public ScreenCaptureApi
{
public:
    ScreenCaptureApiImpl(ScreenCaptureSpi &spi);
    ~ScreenCaptureApiImpl() override;

    int connect() override;
    int queryScreenImage() override;
    void disconnect() override;

private:
    void handleMessage(zmqpp::socket &socket);
    void addSendQueue(nlohmann::json &msg);

    void start();
    void stop();

    zmqpp::context context_;
    zmqpp::socket requestSocket_;
    zmqpp::reactor reactor_;

    std::atomic_bool isStart_;
    std::vector<std::thread> threads_;
    std::queue<nlohmann::json> sendQueue_;
    ScreenCaptureSpi &spi_;
};

#endif // __SCREENCAPTUREAPIIMPL_H__