#ifndef __QUERYSCREENIMAGEHANDLER_H__
#define __QUERYSCREENIMAGEHANDLER_H__

#include "Handler.h"

#include <memory>

namespace zmqpp {
class context;
}

class QueryScreenImageHandler : public Handler
{
public:
    QueryScreenImageHandler(zmqpp::context &context);
    ~QueryScreenImageHandler() override = default;

    bool handle(MessageHelper &request, MessageHelper &response) override;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

#endif // __QUERYSCREENIMAGEHANDLER_H__