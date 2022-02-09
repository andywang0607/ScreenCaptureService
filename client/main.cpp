#include <zmqpp/zmqpp.hpp>
#include <zmqpp/reactor.hpp>

#include "src/ScreenCaptureApi.h"

#include <thread>
#include <chrono>

int main()
{
    ScreenCaptureApi api;
    
    api.connect();
    api.connect();
    api.connect();

    std::this_thread::sleep_for(std::chrono::seconds(30));
    return 0;
}