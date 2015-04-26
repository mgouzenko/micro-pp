#ifndef __MICRO_RESPONSE_HPP__
#define __MICRO_RESPONSE_HPP__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace micro {

    struct cookie;
    struct header;
    /**
     * A **response** object is passed into a url route callback and is handled
     * by the user. A **response** object wraps a HTTP resonse and gives a user 
     * the ability to safely set response headers and message content in the HTTP response. 
     * The **response** object safely populates the data in a reply which is handled gets handled in the
     * request_handler. 
     */
    class response {

        friend class url_route;
        friend class reply;
        friend class request_handler;

    public:

        /**
        * Constructor for a response.
        */
        response();

        /**
         * Append a string to the message content which will appear
         * in the HTTP respnse message.
         * @param message: The message you wish to append to the HTTP content body
         */
        void append_message(const std::string& message);

        /**
         * Set the message content of a HTTP response. This will write over any
         * content that has already bee added. Gives ability to set the mime_type 
         * which defaults to text/html
         * @param message: Content string you want to send in response.
         * @param mime_type: Mime type for response. Defaults to text/html.
        */
        void render_string(const std::string& message, const std::string& mime_type="text/html");

        /**
         * Read the contents of an open filestream and render it in the response.
         * This will close the filestream.
         * @param f: an open filestream
         */
        void render_filestream(std::ifstream& f);

        /**
         * Attempt to find the file at path, and render it in the response, or render a 404.
         * WARNING: This can render any file in the system, so make sure you sanitize any input to this function.
         *
         * @param file_path: the path to the file you'd like to send to the client
         * @return true if a file was found, false if 404 status was set
         */
        bool render_file(std::string file_path);

        /**
         * Set the key and value of cookie to send back to client
         * @param c: Cookie object
         */
        void set_cookie(const cookie& c);

        /**
        * Redirects browser to differnt url endpoint with 307 redirect response
        * @param path: The url path you wish to redirect your clent to
        * @param relative_to_module_entry: If writing redirect in a module, can choose to
        *        redirect to a path at the app level or module level. Defaults to app level. 
        */
        void redirect(const std::string& path, bool relative_to_module_entry = false);

        /**
        * Renders a default HTTP response for a given HTTP status code.
        * Returns 500 if status code does not exist
        * @param status_code: HTTP status code
        * @param message: Custom message to in HTTP response
        */
        void render_status(int status_code, const std::string& message="");

        /**
        * Set the mime type for a HTTP response
        * @param mime_type: Mime type for HTTP response
        */
        void set_mime_type(const std::string& mime_type);

    private:

        /**
         * Body of HTTP response message
         */
        std::string message_;

        /**
         * Entry point that the module is bound to.
         */
        std::string* module_entry_point_;

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

        /**
        * Mime type of HTTP response
        */
        std::string mime_type_;

        /**
        * Get the message string
        */
        const std::string& get_message() const;

        /**
        * Get reference to vector of headers
        */
        const std::vector<header>& get_headers() const;

        /**
        * Add a header to the list of headers
        */
        void add_header(const header& new_header);

        /**
        * Get the status code
        */
        int get_status_code() const;

        /**
        * Return true if status code has been set in response
        */
        bool did_set_status() const;

        /**
        * Return true if message has been set in response
        */
        bool did_set_message() const;

        /**
        * Get the mime type set in the request
        */
        const std::string& get_mime_type() const;

        /**
        * Set the message for the HTTP response
        * @param msg: The message you wish to set in the response
        */
        void set_message(const std::string& msg);

        /**
        * Set the status code of the response
        * @param status_code: Gives user ability to render a status code
        */
        void set_status_code(int status_code);

    };

} // namespace micro

#endif // MICRO_RESPONSE_HPP
