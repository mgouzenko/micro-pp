#ifndef MICRO_RESPONSE_HPP
#define MICRO_RESPONSE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

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
         * Read the contents of an open filestream and render it in the response.
         * This will close the filestream.
         * @param f: an open filestream
         */
        void render_filestream(std::ifstream& f);

        /**
         * Attempt to find the file at path, and render it in the response, or render a 404.
         * WARNING: This can render any file in the system, so make sure you sanitize any input to this function.
         */
        void render_file(std::string file_path);

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
        * Renders a default HTTP response based of status code
        * @param status_code: HTTP status code
        * @param message: Custom message to in HTTP response
        */
        void render_status(int status_code, const std::string& message="");

        /**
        * Get the status code
        */
        int get_status_code() const;

        /**
        * Set the status code
        */
        void set_status_code(int status_code);

        /**
        * Return true if status code has been set in response
        */
        bool did_set_status() const;

        /**
        * Return true if message has been set in response
        */
        bool did_set_message() const;


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
        * Status code to be sent to for default response
        */
        int status_code_;

        /**
        * Indicates whether status has been set in response
        */
        bool did_set_status_;

        /*
        * Indicates whether message has been set in response
        */
        bool did_set_message_;

    };

} // namespace micro

#endif // MICRO_RESPONSE_HPP
