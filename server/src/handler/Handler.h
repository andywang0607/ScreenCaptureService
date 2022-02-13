#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "MessageHelper.hpp"

class Handler
{
public:
    Handler() = default;
    virtual ~Handler() = default;

    virtual bool handle(MessageHelper &request, MessageHelper &response) = 0;

protected:
    void updateAction(MessageHelper &request, MessageHelper &response)
    {
        response.set("action", request.get("action"));
    }

    void updateConnectToken(MessageHelper &request, MessageHelper &response)
    {
        response.setConnectToken(request.getConnectToken());
    }
};

#endif // __HANDLER_H__