#ifndef __SCREENCAPTURESERVER_H__
#define __SCREENCAPTURESERVER_H__

#include <memory>
#include <string>

class ScreenCaptureServer
{
public:
    ScreenCaptureServer();
    ~ScreenCaptureServer();

    void startService();
    bool getIsStart();

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;

};

#endif // __SCREENCAPTURESERVER_H__