#include <vector>
#include <string>
#include <iostream>

#include "response.hpp"
#include "cookie.hpp"
#include "reply.hpp"

namespace micro {

    response::response() 
    {
        send_default_ = false;
        did_set_status_ = false;
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

    void response::set_cookie(const Cookie& c)
    {
        header h = header();
        h.name = "Set-Cookie";
        h.value = c.to_string();
        add_header(h);
    }

    void response::redirect(const std::string& path) 
    {
        set_status_code(301);
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

    void response::render_status(int status_code, const std::string& message)
    {
        status_code = status_code;
        append_message(message);
    }

    bool response::should_send_default() const
    {
        return send_default_;
    }

    int response::get_status_code() const 
    {
        return status_code_;
    }

    void response::set_status_code(int status_code)
    {
        //TODO::Assert that status code exists
        did_set_status_ = true;
        status_code_ = status_code;
    }

    bool response::did_set_status() const 
    {
        return did_set_status_;
    }

}
