#include "src/ScreenCaptureServer.hpp"

#include <chrono>
#include <thread>

int main()
{
    ScreenCaptureServer server;
    server.startService();
    while(server.getIsStart()) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}