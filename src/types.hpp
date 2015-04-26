#ifndef __MICRO_TYPES_HPP__
#define __MICRO_TYPES_HPP__

#include <functional>
#include <vector>

namespace micro {
    struct request;
    struct response;

    typedef std::function<void(const micro::request& req, micro::response& res)> callback;
}

#endif
