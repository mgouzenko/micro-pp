#ifndef __MICRO_REPLY_HPP__
#define __MICRO_REPLY_HPP__

#include <string>
#include <vector>
#include <boost/asio.hpp>


namespace micro {

    struct response;
    struct header;


    /**
     * A reply to be sent to a client.
     */


    class reply
    {
        friend class server;
        friend class request_handler;
        friend class micro_thread;
        /**
        * The status of the reply.
        */
        public:
        enum status_type
        {
            ok = 200,
            created = 201,
            accepted = 202,
            no_content = 204,
            multiple_choices = 300,
            moved_permanently = 301,
            moved_temporarily = 307,
            not_modified = 304,
            bad_request = 400,
            unauthorized = 401,
            forbidden = 403,
            not_found = 404,
            internal_server_error = 500,
            not_implemented = 501,
            bad_gateway = 502,
            service_unavailable = 503
        } status;

        private:
        /**
        * The headers to be included in the reply.
        */
        std::vector<header> headers;

        /**
        * The content to be sent in the reply.
        */
        std::string content;

        /**
        * Vector of cookies to be added to header
        */
        std::vector<std::string> cookies;

        /**
        * Handles a response object and safely sets HTTP message content and headers
        * param response: Response object to be handled
        */
        void handle_response(const response& res);

        /**
        * Convert the reply into a vector of buffers.
        * The buffers do not own the underlying memory blocks, therefore the
        * reply object must remain valid and not be changed until the write operation
        * has completed.
        */
        std::vector<boost::asio::const_buffer> to_buffers();

        /**
        * Get a stock reply defined by the reply class
        * @param status: HTTP status type
        */
        static reply stock_reply(status_type status);

        /**
        * Translate a numeric status code into a status
        * @param code: HTTP status code
        */
        static reply::status_type translate_status_code(int code);

    };


} // namespace micro

#endif // MICRO_REPLY_HPP
