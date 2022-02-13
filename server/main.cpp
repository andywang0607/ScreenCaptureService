#include <thread>
#include <chrono>

#include "src/ScreenCaptureServer.hpp"

int main()
{
    ScreenCaptureServer server;
    server.startService();
    while(server.getIsStart()) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}