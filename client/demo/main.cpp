#include <opencv2/opencv.hpp>

#include "threadsafe_queue.hpp"
#include "include/ScreenCaptureApi.h"

#include <thread>
#include <chrono>
#include <iostream>

static ScreenCaptureApi *api = nullptr;
threadsafe_queue<cv::Mat> imgQueue;

class MySpi : public ScreenCaptureSpi
{
    void onImageRtn(unsigned char *data, int length) override
    {
        std::cout << "onImageRtn\n"
                  << "data length: " << length << "\n";
        auto mat = cv::imdecode(std::vector<unsigned char>(data, data+length), cv::IMREAD_COLOR);
        imgQueue.push(mat);
        std::cout << "mat channels: " << mat.channels() << "\n";
    }

    void onConnectRspRtn(int imgWidth, int imgHeight) override
    {
        std::cout << "onConnectRspRtn\n"
                  << "imgWidth: " << imgWidth << "\n"
                  << "imgHeight: " << imgHeight << "\n";
    }

    void onStartQueryScreenImageRspRtn(const char *msg) override
    {
        std::cout << "onStartQueryScreenImageRspRtn\n"
                  << "msg: " << msg << "\n";
    }

    void onStopQueryScreenImageRspRtn(const char *msg) override
    {
        std::cout << "onStopQueryScreenImageRspRtn\n"
                  << "msg: " << msg << "\n";
    }
    void onDisConnectRspRtn(const char *msg) override
    {
        std::cout << "onDisConnectReturn\n"
                  << "msg: " << msg << "\n";
    }


} mySpi;

int main()
{
    api = ScreenCaptureApi::create(mySpi);
    
    api->connect("192.168.2.88", 8080);
    api->startQueryScreenImage();

    while (true) {
        auto img = imgQueue.wait_and_pop();
        cv::imshow("Screen", *img);

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    api->stopQueryScreenImage();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    api->disconnect();
    
    return 0;
}