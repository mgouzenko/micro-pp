#include <vector>
#include <string>
#include <iostream>

#include "response.hpp"
#include "cookie.hpp"
#include "reply.hpp"

namespace micro {

    response::response() {
        issue_redirect_ = false;
        send_default_ = false;
    }

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

    void response::render_status(int status_code)
    {
        send_default_ = true;
        status_code_ = status_code;
    }

    bool response::should_send_default() const
    {
        return send_default_;
    }

    int response::get_status_code() const 
    {
        return status_code_;
    }

}
