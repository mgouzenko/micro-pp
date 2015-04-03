#include <vector>
#include <string>
#include <iostream>
#include "response.hpp"
#include "cookie.hpp"

namespace micro {

    /*
    * Get the message string
    */
    const std::string& response::get_message() const
    {
        return message_;
    }

    /*
    * Append a string to the message content
    */
    void response::append_message(const std::string& my_message)
    {
        message_.append(my_message);
    }

    /*
    * Get reference to vector of headers
    */
    const std::vector<header>& response::get_headers() const
    {
        return headers_;
    }

    /*
    * Add a header to the list of headers
    */
    void response::add_header(const header& new_header)
    {
        headers_.push_back(new_header);
    }

    /*
    * Sets the message content of a HTTP response
    * param message: Content string you want to send in response
    */
    void response::render_string(std::string message)
    {
        append_message(message);
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

}
