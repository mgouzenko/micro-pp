#ifndef MICRO_TYPES_HPP
#define MICRO_TYPES_HPP

#include <functional>
#include <vector> 

namespace micro {
    struct request; 
    struct response; 

    typedef std::function<void(const micro::request& req, micro::response& res)> callback;
}

#endif
