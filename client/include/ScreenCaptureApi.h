#ifndef __SCREENCAPTUREAPI_H__
#define __SCREENCAPTUREAPI_H__

struct ScreenCaptureSpi
{
    virtual void onConnectReturn(int imgWidth, int imgHeight){};
    virtual void onImgReturn(unsigned char *data, int length){};
    virtual void onDisConnectReturn(char *msg){};

    virtual ~ScreenCaptureSpi(){};
};

struct ScreenCaptureApi
{
    static ScreenCaptureApi *create(ScreenCaptureSpi &spi);
    virtual ~ScreenCaptureApi(){};

    virtual int connect() = 0;
    virtual int queryScreenImage() = 0;
    virtual void disconnect() = 0;
};

#endif // __SCREENCAPTUREAPI_H__