#include <iostream>
#include <string>
#include <chrono>

#include "util/screencapture/ScreenCapture.h"

int main()
{
    ScreenCapture capture;

    cv::namedWindow("Screen", cv::WINDOW_AUTOSIZE);

    while (true) {
        auto &&[width, height] = capture.getCurrentScreenSize();

        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        auto ret = capture.captureScreenRect(0, 0, width, height);
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        std::cout << "captureScreenRect time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "\n";

        if (ret.empty()) {
            continue;
        }

        cv::imshow("Screen", ret);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
}