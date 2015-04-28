#ifndef __MICRO_TYPES_HPP__
#define __MICRO_TYPES_HPP__

#include <functional>
#include <vector>

namespace micro {
    struct request;
    struct response;

    typedef std::function<micro::response(const micro::request& req)> callback;
}

#endif
