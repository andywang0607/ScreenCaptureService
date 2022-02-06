#include <iostream>
#include <string>
#include <chrono>

#include "ScreenCapture.h"
#include "base64.h"
#include "opencv2/opencv.hpp"

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
		std::cout << "ret size: " << ret.size() << "\n";
        if (ret.empty()) {
            continue;
        }
        auto base64Str = base64_encode(ret.data(), ret.size());
        auto decodeStr = base64_decode(base64Str);
		auto mat = cv::imdecode(std::vector<unsigned char>(decodeStr.begin(), decodeStr.end()), cv::IMREAD_COLOR);
		std::cout<< "channels: " << mat.channels() << "\n";
        cv::imshow("Screen", mat);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
}