
#ifndef __TYPES_HPP__
#define __TYPES_HPP__
#include "request.hpp"
#include "reply.hpp"
#include "response.hpp"
#include <functional>
#include <utility>

namespace micro {

    typedef std::function<void(const micro::request& req, micro::response& res)>  callback;

}

#endif
