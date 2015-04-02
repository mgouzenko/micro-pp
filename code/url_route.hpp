#include <regex>
#include "types.hpp"

namespace micro{

    /**
     * A representation of a URL path that can be registered with a Micro++ app.
     */
    class url_route{

    public:

        /**
         * Catch-all URL constructor.
         * Constructs a URL object that will match any valid URL
         *
         * @param methods the HTTP methods that this URL path allows (e.g. {"GET", "POST"})
         * @param callback the callback function that should be registered to this URL path
                  The callback must be a void function that takes a micro::request, and micro::response& as parameters.
         */
        url_route(std::vector<std::string> methods, Callback callback); // Static file URL

        /**
         * Specific URL constructor.
         * Constructs a URL object that will match URLs that fit the specifier parameter
         *
         * The specifier string can define either absolute paths such as:
         * "/this/is/an/absolute/path"
         *
         * Or it can define paths with variable, capturable components, each having a label.
         * For example, the following specifier:
         * "/user/<username>/profile"
         * will match URLs such as "/user/johndoe/profile" or "/user/cooldude42/profile"
         *
         * A specifier can also restrict the types of values allowed in a URL.
         * For example, the following specifier:
         * "/user/<int:id>/profile"
         * will match URLs like "/user/5151/profile", but not "/user/adamchel/profile"
         *
         * Note that specifiers are strict about trailing slashes.
         * For example. "/this/is/cool/" is not the same as "/this/is/cool"
         *
         * @param specifier a string that defines what kind of URLs the instance should accept
         * @param methods the HTTP methods that this URL path allows (e.g. {"GET", "POST"})
         * @param callback the callback function that should be registered to this URL path
                  The callback must be a void function that takes a micro::request, and micro::response& as parameters.
         */
        url_route(std::string specifier, std::vector<std::string> allowable_methods, Callback callback);

        /**
         * Checks if the URI within a request matches this URL instance.
         *
         * @param request a request containing a sanitized (no query string) URI object
         * @return true if the request matches the URL, false otherwise.
                   If match() returns true, the request object will also be populated with any captured label-value pairs
         */
        bool match(http::server4::request& request);

        /**
         * Executes the callback registered with this URL.
         * *callback() should only be executed if match() returns true for the given request object.*
         *
         * @param request the HTTP request to execute the callback with
         * @param reply the HTTP response that will be populated by the callback
         */
        void callback(const http::server4::request& request, http::server4::response& response);

    private:
        std::regex internal_regex_;
        std::vector<std::string> methods_;
        std::vector<std::string> labels_;
        Callback callback_;
    };

}
