#include <string>
#include <iostream>
#include "cookie.hpp"

namespace micro {

    cookie::cookie(const std::string& key, const std::string& val, const std::string path, const std::string domain)
    : key_{key}, val_{val}, path_{path}, domain_{domain} {};

    cookie::cookie(const std::string& key, const std::string& val, const std::time_t& expires, 
      const std::string path, const std::string domain)
    : key_{key}, val_{val}, expires_{expires}, path_{path}, domain_{domain} 
    {
        add_expires_ = true;
    };

    std::string cookie::to_string() const
    {
        std::string cookie_string = key_ + "=" + val_;

        if (add_expires_) {
          tm *gmtm = gmtime(&expires_);
          char *dt = asctime(gmtm);
          std::string curr_time = std::string(dt);

          //Remove newline
          curr_time = curr_time.substr(0, curr_time.size()-1);
          std::string time_string = "; Expires=" + curr_time;
          cookie_string.append(time_string);
        }

        std::string path_string = "; Path=" + path_;
        cookie_string.append(path_string);

        if (!domain_.empty()) {
          std::string domain_string = "; Domain=" + domain_;
          cookie_string.append(domain_string);
        }

        return cookie_string;
    }

}
