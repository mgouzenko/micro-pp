#include <vector>
#include <string>
#include <iostream>

#include "response.hpp"
#include "cookie.hpp"

namespace micro {

    const std::string& response::get_message() const
    {
        return message_;
    }

    void response::append_message(const std::string& my_message)
    {
        message_.append(my_message);
    }

    const std::vector<header>& response::get_headers() const
    {
        return headers_;
    }

    void response::add_header(const header& new_header)
    {
        headers_.push_back(new_header);
    }

    void response::render_string(std::string message)
    {
        append_message(message);
    }


    bool response::should_redirect() const 
    {
        return issue_redirect_;
    }

    /*
    * Set the key and value of cookie to send back to client
    * param key: key of cookie
    * param value: value of cookie
    */
    void response::set_cookie(const Cookie& c)
    {
        header h = header();
        h.name = "Set-Cookie";
        h.value = c.to_string();
        add_header(h);
    }

    void response::redirect(const std::string& path) 
    {
        issue_redirect_ = true;
        header h = header();
        h.name = "Location";
        h.value = path;
        add_header(h);
    }

}
