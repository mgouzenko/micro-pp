#include <iostream>
#include "request.hpp"
#include <unordered_map>
#include <regex>

namespace micro {

    std::string request::get_cookie(const std::string& key)
    {
        if(cookies.find(key) == cookies.end()) {
          return "";
        }
        else {
          return std::string(cookies[key]);
        }
    }

    const std::string& request::get_hostname() const
    {
        return hostname;
    }

    const std::string& request::get_uri() const
    {
        return uri;
    }

    const std::string& request::get_ip() const
    {
        return client_ip;
    }

    const std::string& request::get_method() const
    {
        return method;
    }

}
