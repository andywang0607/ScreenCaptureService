#ifndef __CONNECTHANDLER_H__
#define __CONNECTHANDLER_H__

#include "Handler.h"
#include "ScreenCapture.h"

class ConnectHandler : public Handler
{
public:
    ConnectHandler() = default;
    ~ConnectHandler() override = default;

    bool handle(MessageHelper &request, MessageHelper &response) override;

private:
    ScreenCapture screenCapture_;
};

#endif // __CONNECTHANDLER_H__