#include <zmqpp/zmqpp.hpp>
#include <zmqpp/reactor.hpp>
#include <opencv2/opencv.hpp>
#include "threadsafe_queue.hpp"

#include "include/ScreenCaptureApi.h"
#include "src/ScreenCaptureApiImpl.h"

#include <thread>
#include <chrono>
#include <iostream>

static ScreenCaptureApi *api = nullptr;
threadsafe_queue<cv::Mat> imgQueue;

class MySpi : public ScreenCaptureSpi
{
    void onImgReturn(unsigned char *data, int length) override
    {
        std::cout << "onImgReturn\n"
                  << "data length: " << length << "\n";
        auto mat = cv::imdecode(std::vector<unsigned char>(data, data+length), cv::IMREAD_COLOR);
        imgQueue.push(mat);
        std::cout << "mat channels: " << mat.channels() << "\n";
    }

    void onConnectReturn(int imgWidth, int imgHeight) override
    {
        std::cout << "onConnectReturn\n"
                  << "imgWidth: " << imgWidth << "\n"
                  << "imgHeight: " << imgHeight << "\n";
    }

    void onDisConnectReturn(char *msg) override
    {
        std::cout << "onDisConnectReturn\n"
                  << "msg: " << msg << "\n";
    }


} mySpi;

int main()
{
    api = ScreenCaptureApi::create(mySpi);
    
    api->connect();
    api->queryScreenImage();

    auto img = imgQueue.wait_and_pop();
    cv::imshow("Screen", *img);

    while (true) {
        if (cv::waitKey(0) == 'q') {
            break;
        }
    }

    api->disconnect();
    
    return 0;
}