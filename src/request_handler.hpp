#ifndef MICRO_REQUEST_HANDLER_HPP
#define MICRO_REQUEST_HANDLER_HPP

#include <string>
#include <unordered_map>
#include <functional>

#include "types.hpp"
#include "server.hpp"
#include "response.hpp"
#include "url_route.hpp"

namespace micro {

    struct reply;
    struct request;

    /**
     * The common handler for all incoming requests.
     */
    class request_handler
    {
    public:

      /**
       * Handle a request and produce a reply.
       */
      void operator()(server& serv);

      /**
       * Add a URL route with a registered callback.
       */
      void add_route(micro::url_route);

      /**
       * Sets the filepath where the handler's serve_static() will look for files
       */
      void set_static_root(std::string root);

      /**
       * Serves a static file.
       * Intended to be the micro::callback for the catch-all route automatically added by app.run()
       */
       void serve_static(const micro::request& req, micro::response& resp);

       void set_debug_mode();

       
      /**
       * Perform URL-decoding on a string. Returns false if the encoding was invalid.
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
      std::vector<micro::url_route> callback_routes_;

      bool debug_mode_ = false; 
    };

} // namespace micro

#endif // MICRO_REQUEST_HANDLER_HPP
