#ifndef __MICRO_COOKIE_HPP__
#define __MICRO_COOKIE_HPP__

#include <string>
#include <ctime>

namespace micro {

    /**
     * A model of an HTTP cookie.
     * A cookie object models an HTTP cookie. It is placed in the header
     * of a HTTP response.
     */
    class cookie {

    private:

        /**
        * Key of the cookie
        */
        std::string key_;

        /**
        * Value of the cookie
        */
        std::string val_;

        /**
        * The time you wish the cookie to expire
        */
        std::time_t expires_;

        /**
        * The domian for which the the cookie has access to
        */
        std::string domain_;

        /*
        * The path of on the domain the cookie has access to
        */
        std::string path_;

        /**
        * Boolean to indicate if user set expiry time
        */
        bool add_expires_ = false;

    public:
        /**
        * Constructor for cookie.
        * @param key: key of cookie
        * @param val: val of cookie
        * @param path: path cookie will be sent for
        * @param domain: domain accessible by cookie
        */
        cookie(const std::string& key, const std::string& val, const std::string path="/", const std::string domain="");

        /**
        * Constructor for cookie with exipiration.
        * @param key: key for cookie
        * @param val: value of cookie
        * @param expires: date that cookie expires
        * @param path: path cookie will be sent for
        * @param domain: domain accessible by cookie
        */
        cookie(const std::string& key, const std::string& val, const std::time_t& expires,
            const std::string path="/", const std::string domain="");

        /**
        * Converts the cookie to an HTTP formatted string.
        */
        std::string to_string() const;

    };

} // namespace micro

#endif // MICRO_COOKIE_HPP
