/*
***************
* cookie.hpp
***************
* 
* A cookie object models an HTTP cookie. It is placed in the header
* of a HTTP response.
*/
#ifndef HTTP_COOKIE_HPP
#define HTTP_COOKIE_HPP

#include <string>
#include <iostream>

namespace http {
namespace server4 {

class Cookie
{

private:

    std::string key_;

    std::string val_;

    std::time_t expires_;

public:
    /*
    * Constructor for Cookie
    * param key: key of cookie
    * param val: val of cookie
    */
    Cookie(const std::string& key, const std::string& val);

    /*
    * Constructor
    * param key: key for cookie
    * param val: value of cookie
    * param expires: date that cookie expires
    */
    Cookie(const std::string& key, const std::string& val, const std::time_t expires);

    /*
    * Converts Cookie to http formatted string
    */ 
    std::string to_string() const;

};

} // namespace server4
} // namespace http

#endif // HTTP_COOKIE_HPP