#ifndef __SCREENCAPTUREAPI_H__
#define __SCREENCAPTUREAPI_H__

#include "screencaptureapi_export.h"

struct ScreenCaptureSpi
{
    /**
     * @brief Response of connect
     *
     * @param imgWidth width of screen resolution
     * @param imgHeight height of screen resolution
     */
    virtual void onConnectRspRtn(int imgWidth, int imgHeight){};

    /**
     * @brief Response of startQueryScreenStream
     *
     * @param msg reply message
     */
    virtual void onStartQueryScreenStreamRspRtn(const char *msg){};

    /**
     * @brief Response of stopQueryScreenStream
     *
     * @param msg reply message
     */
    virtual void onStopQueryScreenStreamRspRtn(const char *msg){};

    /**
     * @brief Response of disconnect
     *
     * @param msg reply message
     */
    virtual void onDisConnectRspRtn(const char *msg){};

    /**
     * @brief Screen image stream return
     *
     * @param data image data (after jpeg encode)
     * @param length data length
     */
    virtual void onStreamRtn(unsigned char *data, int length){};

    /**
     * @brief Single screenshot image return
     *
     * @param data image data (after jpeg encode)
     * @param length data length
     */
    virtual void onImageRtn(unsigned char *data, int length){};

    virtual ~ScreenCaptureSpi(){};
};

struct ScreenCaptureApi
{
    /**
     * @brief Create instance of api
     *
     * @param spi your own SPI
     * @return ScreenCaptureApi*
     */
    SCREENCAPTUREAPI_EXPORT static ScreenCaptureApi *create(ScreenCaptureSpi &spi);
    SCREENCAPTUREAPI_EXPORT virtual ~ScreenCaptureApi(){};

    /**
     * @brief connect to server
     *
     * @param ip server ip
     * @param port server port
     * @return error code 0 is success
     */
    SCREENCAPTUREAPI_EXPORT virtual int connect(const char *ip, int port) = 0;

    /**
     * @brief query a single screenshot image
     *
     * @param targetWidth return image width, default -1(origin size)
     * @param targetHeight return image height, default -1(origin size)
     * @return error code 0 is success
     */
    SCREENCAPTUREAPI_EXPORT virtual int queryScreenImage(int targetWidth = -1, int targetHeight = -1) = 0;

    /**
     * @brief start query screen image stream
     *
     * @param targetWidth return image width, default -1(origin size)
     * @param targetHeight return image height, default -1(origin size)
     * @return error code 0 is success
     */
    SCREENCAPTUREAPI_EXPORT virtual int startQueryScreenStream(int targetWidth = -1, int targetHeight = -1) = 0;

    /**
     * @brief stop query screen image stream
     *
     * @return error code 0 is success
     */
    SCREENCAPTUREAPI_EXPORT virtual int stopQueryScreenStream() = 0;

    /**
     * @brief disconnect to server
     *
     * @return error code 0 is success
     */
    SCREENCAPTUREAPI_EXPORT virtual void disconnect() = 0;
};

#endif // __SCREENCAPTUREAPI_H__