#ifndef MICRO_COOKIE_HPP
#define MICRO_COOKIE_HPP

#include <string>
#include <iostream>

namespace micro {

    /**
     * A model of an HTTP cookie.
     * A cookie object models an HTTP cookie. It is placed in the header
     * of a HTTP response.
     */
    class Cookie {

    private:

        std::string key_;

        std::string val_;

        std::time_t expires_;

        std::string domain_;

        std::string path_;

        bool add_expires_ = false;

    public:
        /**
        * Constructor for Cookie.
        * @param key: key of cookie
        * @param val: val of cookie
        * @param path: path cookie will be sent for
        * @param domain: domain accessible by cookie
        */
        Cookie(const std::string& key, const std::string& val, const std::string path="/", const std::string domain="");

        /**
        * Constructor for Cookie with exipiration.
        * @param key: key for cookie
        * @param val: value of cookie
        * @param expires: date that cookie expires
        * @param path: path cookie will be sent for
        * @param domain: domain accessible by cookie
        */
        Cookie(const std::string& key, const std::string& val, const std::time_t& expires, 
            const std::string path="/", const std::string domain="");

        /**
        * Converts the Cookie to an HTTP formatted string.
        */
        std::string to_string() const;

    };

} // namespace micro

#endif // MICRO_COOKIE_HPP
