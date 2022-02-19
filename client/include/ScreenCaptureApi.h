#ifndef __SCREENCAPTUREAPI_H__
#define __SCREENCAPTUREAPI_H__

#include "screencaptureapi_export.h"

struct ScreenCaptureSpi
{
    virtual void onConnectRspRtn(int imgWidth, int imgHeight){};
    virtual void onStartQueryScreenStreamRspRtn(const char *msg){};
    virtual void onStopQueryScreenStreamRspRtn(const char *msg){};
    virtual void onDisConnectRspRtn(const char *msg){};

    virtual void onStreamRtn(unsigned char *data, int length){};
    virtual void onImageRtn(unsigned char *data, int length){};

    virtual ~ScreenCaptureSpi(){};
};

struct ScreenCaptureApi
{
    SCREENCAPTUREAPI_EXPORT static ScreenCaptureApi *create(ScreenCaptureSpi &spi);
    SCREENCAPTUREAPI_EXPORT virtual ~ScreenCaptureApi(){};

    SCREENCAPTUREAPI_EXPORT virtual int connect(const char *ip, int port) = 0;
    SCREENCAPTUREAPI_EXPORT virtual int queryScreenImage(int targetWidth = -1, int targetHeight = -1) = 0;
    SCREENCAPTUREAPI_EXPORT virtual int startQueryScreenStream(int targetWidth = -1, int targetHeight = -1) = 0;
    SCREENCAPTUREAPI_EXPORT virtual int stopQueryScreenStream() = 0;
    SCREENCAPTUREAPI_EXPORT virtual void disconnect() = 0;
};

#endif // __SCREENCAPTUREAPI_H__