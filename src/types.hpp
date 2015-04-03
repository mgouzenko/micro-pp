
#ifndef MICRO_TYPES_HPP
#define MICRO_TYPES_HPP

#include <functional>
#include <utility>

#include "request.hpp"
#include "reply.hpp"
#include "response.hpp"


namespace micro {

    typedef std::function<void(const micro::request& req, micro::response& res)> callback;

}

#endif
