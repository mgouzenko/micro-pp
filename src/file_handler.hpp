#ifndef MICRO_FILE_HANDLER_HPP
#define MICRO_FILE_HANDLER_HPP

#include <string>

namespace micro {

    struct reply;
    struct request;

    /**
     * The common handler for all incoming requests.
     */
    class file_handler {
    public:
      /**
       * Constructor with a directory containing files to be served.
       */
      explicit file_handler (const std::string& doc_root);

      /**
       * Handles a request and produce a reply.
       */
      void operator()(const request& req, reply& rep);

    private:
      /**
       * The directory containing the files to be served.
       */
      std::string doc_root_;

      /**
       * Perform URL-decoding on a string. Returns false if the encoding was invalid.
       */
      static bool url_decode(const std::string& in, std::string& out);
    };

} // namespace micro

#endif // MICRO_FILE_HANDLER_HPP
