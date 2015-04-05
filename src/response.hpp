#ifndef MICRO_RESPONSE_HPP
#define MICRO_RESPONSE_HPP

#include <string>
#include <vector>

#include "header.hpp"
#include "cookie.hpp"

namespace micro {

    /**
     * A response object.
     * A response object is passed into a url route callback and is handled
     * by the user. A user has the ability to safely set response headers and
     * message content in the HTTP response. Response object safely populates
     * the data in a reply which is handled in the request_handler
     */
    class response {
    public:

        /**
        * Constructor for response
        */
        response();

        /**
         * Get the message string
         */
        const std::string& get_message() const;

        /**
         * Append a string to the message content
         */
        void append_message(const std::string& message);

        /**
         * Get reference to vector of headers
         */
        const std::vector<header>& get_headers() const;

        /**
         * Add a header to the list of headers
         */
        void add_header(const header& new_header);

        /**
         * Sets the message content of a HTTP response
         * @param message: Content string you want to send in response
        */
        void render_string(std::string message);

        /**
         * Set the key and value of cookie to send back to client
         * @param c: Cookie object
         */
        void set_cookie(const Cookie& c);

        /**
        * Redirects browser to differnt url endpoint with 301 Found response
        */
        void redirect(const std::string& path);

        /**
        * Return true to signify that 301 redirect should happen
        */
        bool should_redirect() const;

        /**
        * Renders a default HTTP response based of status code
        * @param status_code: HTTP status code
        */
        void render_status(int status_code); 

        /**
        * Return true if should send a predifed HTTP response
        */
        bool should_send_default() const;

        /**
        * Get the status code
        */
        int get_status_code() const;


    private:

        /**
         * Body of HTTP response message
         */
        std::string message_;

        /**
         * Customizable vector or headers
         */
        std::vector<header> headers_;

        /**
        * Boolean to determine if redirect should be issued
        */
        bool issue_redirect_;

        /**
        * Boolean to determine if should send a default reply
        */
        bool send_default_;

        /**
        * Status code to be sent to for default response
        */
        int status_code_;

    };

} // namespace micro

#endif // MICRO_RESPONSE_HPP
