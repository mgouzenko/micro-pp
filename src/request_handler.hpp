#ifndef __MICRO_REQUEST_HANDLER_HPP__
#define __MICRO_REQUEST_HANDLER_HPP__

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

#include "types.hpp"

namespace micro {
    struct url_route;
    struct response;
    struct reply;
    struct request;
    struct server;

    /**
     * The common handler for all incoming requests.
     */
    class request_handler{

    public:

        /**
        * Handle a request and produce a reply.
        */
        void operator()(server& serv);


        /**
        * Adds a callback url route to the list of routes this application can service.
        * Routes are matched in the order in which they were installed. If there is a
        * collision between a URL route and a file within the static folder, the file
        * in the static folder will always be matched to the URL first.
        * @param route: the url_route object representing the route to be added.
        */
        void add_route(url_route);

        /**
        * Set the static directory, the root directory from which static
        * files will be rendered. By default, this is ./static/
        * @param root: the root of the static file directory
        */
        void set_static_root(std::string root);

        /**
        * Serves a static file.
        * Intended to be the micro::callback for the catch-all route automatically added by app.run()
        */
        void serve_static(const request& req, response& resp);


        /**
         * Toggles debug mode. When the app is in debug mode, all uncaught exceptions will be
         * displayed in the browser.
         */
        void set_debug_mode(bool toggle) { debug_mode_ = toggle; }

        /**
         * Toggles loging mode.
         */
        void set_logging_mode(bool toggle) { logging_mode_ = toggle; }

        /**
        * Takes a url that has encoded special characters and decodes to find
        * the correct decoding.
        * @param in: encoded url from request
        * @param out: decoded url
        * @return bool: true on success, false on fail
        */
        static bool url_decode(const std::string& in, std::string& out);

    private:

        /**
        * The directory containing the files to be served.
        */
        std::string static_root_ = "./static/";

        /**
        * The list of url_routes containing callbacks, in order of priority
        */
        std::vector<url_route> callback_routes_;

        /**
         * The flag indicating whether or not debug mode is on. By default, it's off.
         * In debug mode, any uncaught exceptions are displayed in the browser.
         */
        bool debug_mode_ = false;

        /**
         * The flag indicating whether or not logging mode is on (on by default). When logging mode is on,
         * all requests are logged in the console. Users are advised to redirect program output
         * to a log file if they would like to save server logs.
         */
        bool logging_mode_ = true;

        /**
         * Invokes server so that a reply may be sent to the client.
         */
        void invoke_server(server& serv);

    };

} // namespace micro

#endif // MICRO_REQUEST_HANDLER_HPP
