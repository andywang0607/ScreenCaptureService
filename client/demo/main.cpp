#include <opencv2/opencv.hpp>

#include "threadsafe_queue.hpp"
#include "include/ScreenCaptureApi.h"

#include <thread>
#include <chrono>

#include <spdlog/spdlog.h>

static ScreenCaptureApi *api = nullptr;
threadsafe_queue<cv::Mat> imgQueue;

class MySpi : public ScreenCaptureSpi
{
    void onStreamRtn(unsigned char *data, int length) override
    {
        auto mat = cv::imdecode(std::vector<unsigned char>(data, data+length), cv::IMREAD_COLOR);
        imgQueue.push(mat);
    }

    void onImageRtn(unsigned char *data, int length) override
    {
        auto mat = cv::imdecode(std::vector<unsigned char>(data, data + length), cv::IMREAD_COLOR);
        cv::imwrite("screen.jpg", mat);
    }

    void onConnectRspRtn(int imgWidth, int imgHeight) override
    {
        spdlog::info("[MySpi] onConnectRspRtn\n imgWidth: {}\n imgHeight: {}", imgWidth, imgHeight);
    }

    void onStartQueryScreenStreamRspRtn(const char *msg) override
    {
        spdlog::info("[MySpi] onStartQueryScreenStreamRspRtn\n msg: {}", msg);
    }

    void onStopQueryScreenStreamRspRtn(const char *msg) override
    {
        spdlog::info("[MySpi] onStopQueryScreenStreamRspRtn\n msg: {}", msg);
    }
    void onDisConnectRspRtn(const char *msg) override
    {
        spdlog::info("[MySpi] onDisConnectRspRtn\n msg: {}", msg);
    }


} mySpi;

int main()
{
    api = ScreenCaptureApi::create(mySpi);
    
    api->connect("127.0.0.1", 8080);
    api->queryScreenImage();
    api->startQueryScreenStream();

    while (true) {
        auto img = imgQueue.wait_and_pop();
        cv::imshow("Screen", *img);

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    api->stopQueryScreenStream();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    api->disconnect();
    
    return 0;
}