#ifndef __SCREENCAPTUREAPI_H__
#define __SCREENCAPTUREAPI_H__

struct ScreenCaptureSpi
{
    virtual void onConnectRspRtn(int imgWidth, int imgHeight){};
    virtual void onStartQueryScreenImageRspRtn(const char *msg){};
    virtual void onStopQueryScreenImageRspRtn(const char *msg){};
    virtual void onDisConnectRspRtn(const char *msg){};

    virtual void onImageRtn(unsigned char *data, int length){};

    virtual ~ScreenCaptureSpi(){};
};

struct ScreenCaptureApi
{
    static ScreenCaptureApi *create(ScreenCaptureSpi &spi);
    virtual ~ScreenCaptureApi(){};

    virtual int connect(const char *ip, int port) = 0;
    virtual int startQueryScreenImage() = 0;
    virtual int stopQueryScreenImage() = 0;
    virtual void disconnect() = 0;
};

#endif // __SCREENCAPTUREAPI_H__