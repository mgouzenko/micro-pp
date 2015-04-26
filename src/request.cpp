#include "request.hpp"

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
        try {
            return post_params_.at(key);
        }
        catch (const std::out_of_range &oor) {
            return "";
        }
    }

    std::string request::get_query_param(const std::string& key) const
    {
        try {
            return get_params_.at(key);
        }
        catch (const std::out_of_range &oor) {
            return "";
        }
    }

    std::string request::get_route_param(const std::string& key) const
    {
        try {
            return label_values.at(key);
        }
        catch (const std::out_of_range &oor) {
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
