#include "cookie.hpp"
#include <string>

namespace http {
namespace server4 {

    Cookie::Cookie(const std::string& key, const std::string& val) : key_{key}, val_{val} { };

    Cookie::Cookie(const std::string& key, const std::string& val, const std::time_t expires)
    : key_{key}, val_{val}, expires_{expires} { };

    std::string Cookie::to_string() const
    {
        std::string cookie_string = key_ + "=" + val_;

        if (expires_) {
          tm *gmtm = gmtime(&expires_);
          char *dt = asctime(gmtm);
          std::string curr_time = std::string(dt);

          //Remove newline
          curr_time = curr_time.substr(0, curr_time.size()-1);
          std::string time_string = "; Expires=" + curr_time;
          cookie_string.append(time_string);
        }

        return cookie_string;
    }

}
}
