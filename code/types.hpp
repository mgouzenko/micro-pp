
#ifndef __TYPES_HPP__
#define __TYPES_HPP__
#include "request.hpp"
#include "reply.hpp"
#include <functional>

typedef std::function<void(const http::server4::request& req, http::server4::reply& rep)>  Callback;
#endif
