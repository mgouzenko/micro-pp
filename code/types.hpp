
#ifndef __TYPES_HPP__
#define __TYPES_HPP__
#include "request.hpp"
#include <functional>
#include <utility> 

typedef std::function<std::string(const http::server4::request&)>  Callback;

#endif
