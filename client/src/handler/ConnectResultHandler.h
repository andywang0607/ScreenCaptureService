#ifndef __CONNECTRESULTHANDLER_H__
#define __CONNECTRESULTHANDLER_H__

#include "Handler.h"

class ConnectResultHandler : public Handler
{
public:
    ConnectResultHandler() = default;
    ~ConnectResultHandler() override = default;

    bool handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl* api) override;
};

#endif // __CONNECTRESULTHANDLER_H__