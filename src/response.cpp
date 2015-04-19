#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "response.hpp"
#include "cookie.hpp"
#include "header.hpp"
#include "mime_types.hpp"

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
        std::ifstream f;
        f.open(file_path);
        if(f.is_open()) {
            // Add the contents of the file to the message
            render_filestream(f);

            // Determine the file extension.
            std::size_t last_slash_pos = file_path.find_last_of("/");
            std::size_t last_dot_pos = file_path.find_last_of(".");
            std::string extension;
            if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
              extension = file_path.substr(last_dot_pos + 1);
            }

            set_mime_type(micro::mime_types::extension_to_type(extension));
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

    void response::redirect(const std::string& path, bool relative_to_module_entry)
    {
        set_status_code(307);
        header h = header();
        h.name = "Location";
        if(relative_to_module_entry){
            h.value = *module_entry_point_ + path;
        }else{
            h.value = path;
        }
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
