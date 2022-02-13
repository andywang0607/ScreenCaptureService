#ifndef __SUBSCRIPTIONHANDLER_H__
#define __SUBSCRIPTIONHANDLER_H__

#include "Handler.h"

class SubscriptionHandler : public Handler
{
public:
    SubscriptionHandler() = default;
    ~SubscriptionHandler() override = default;

    bool handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl* api) override;
};

#endif // __SUBSCRIPTIONHANDLER_H__