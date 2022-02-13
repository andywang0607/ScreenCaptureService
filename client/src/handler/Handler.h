#ifndef __HANDLER_H__
#define __HANDLER_H__

class MessageHelper;
class ScreenCaptureSpi;
class ScreenCaptureApiImpl;

class Handler
{
public:
    Handler() = default;
    virtual ~Handler() = default;

    virtual bool handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl* api) = 0;
};

#endif // __HANDLER_H__