#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "response.hpp"
#include "cookie.hpp"
#include "header.hpp"

namespace micro {

    response::response()
    {
        did_set_status_ = false;
        did_set_message_ = false;
        message_ = std::string();
        headers_ = std::vector<header>();
    }

    const std::string& response::get_message() const
    {
        return message_;
    }

    void response::append_message(const std::string& my_message)
    {
        did_set_message_ = true;
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

    void response::render_string(const std::string& message, const std::string& mime_type)
    {
        set_mime_type(mime_type);
        set_message(message);
    }

    void response::render_filestream(std::ifstream& f)
    {
        // TODO: Async buffer usage so we don't use a ton of memory for large files (1.2)

        char buf[512];
        while (f.read(buf, sizeof(buf)).gcount() > 0) {
          message_.append(buf, f.gcount());
        }

        did_set_message_ = true;

        f.close();
    }

    bool response::render_file(std::string file_path)
    {
        // TODO: Add support for extracting MIME type (1.0)
        std::ifstream f;
        f.open(file_path);
        if(f.is_open()) {
            render_filestream(f);
            return true;
        }
        set_status_code(404);
        return false;
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

    void response::render_status(int status_code, const std::string& message)
    {
        set_mime_type("text/html");
        set_status_code(status_code);
        if (!message.empty()) {
            set_message(message);
        }
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

    bool response::did_set_message() const
    {
        return did_set_message_;
    }

    void response::set_message(const std::string& msg)
    {
        did_set_message_ = true;
        message_ = std::string(msg);
    }

    void response::set_mime_type(const std::string& mime)
    {
        mime_type_ = std::string(mime);
    }

    const std::string& response::get_mime_type() const
    {
        return mime_type_;
    }

}
