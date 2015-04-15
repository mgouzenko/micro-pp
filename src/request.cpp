#include <iostream>
#include "request.hpp"
#include <unordered_map>
#include <regex>

namespace micro {

    std::string request::get_cookie(const std::string& key) const
    {
        auto search = cookies_.find(key);
        if (search != cookies_.end()) {
            return std::string(search->second);
        }
        else {
            return "";
        }
    }

    std::string request::get_post_param(const std::string& key) const
    {
        auto search = post_params_.find(key);
        if (search != post_params_.end()) {
            return std::string(search->second);
        }
        else {
            return "";
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
