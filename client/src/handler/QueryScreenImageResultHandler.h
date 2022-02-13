#ifndef __QUERYSCREENIMAGERESULTHANDLER_H__
#define __QUERYSCREENIMAGERESULTHANDLER_H__

#include "Handler.h"

class QueryScreenImageResultHandler : public Handler
{
public:
    QueryScreenImageResultHandler() = default;
    ~QueryScreenImageResultHandler() override = default;

    bool handle(MessageHelper &receive, ScreenCaptureSpi *spi, ScreenCaptureApiImpl* api) override;
};

#endif // __QUERYSCREENIMAGERESULTHANDLER_H__