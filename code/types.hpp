
#ifndef __TYPES_HPP__
#define __TYPES_HPP__
#include "request.hpp"
#include "reply.hpp"
#include "response.hpp"
#include <functional>
#include <utility> 

typedef std::function<void(const http::server4::request& req, http::server4::response& res)>  Callback;
#endif
