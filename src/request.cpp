#include <iostream>
#include "request.hpp"
#include <unordered_map>
#include <regex>

namespace micro {

    std::string request::get_cookie(const std::string& key)
    {
        if (cookies_.find(key) == cookies_.end()) {
            return "";
        }
        else {
            return std::string(cookies_[key]);
        }
    }

    std::string request::get_post_param(const std::string& key)
    {
        if (post_params_.find(key) == post_params_.end()) {
            return "";
        }
        else {
            return std::string(post_params_[key]);
        }
    }

    const std::string& request::get_hostname() const
    {
        return hostname_;
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
