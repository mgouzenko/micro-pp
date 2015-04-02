#ifndef HTTP_SERVER4_REQUEST_HPP
#define HTTP_SERVER4_REQUEST_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "header.hpp"

namespace http {
namespace server4 {

    /// A request received from a client.
    struct request
    {
      /// The request method, e.g. "GET", "POST".
      std::string method;

      /// The requested URI, such as a path to a file.
      std::string uri;

      /// Major version number, usually 1.
      int http_version_major;

      /// Minor version number, usually 0 or 1.
      int http_version_minor;

      /// The headers included with the request.
      std::vector<header> headers;

      /// The optional content sent with the request.
      std::string content;

      // Fields populated after the URL is matched:
      std::unordered_map<std::string, std::string> label_values;
    };

} // namespace server4
} // namespace http

#endif // HTTP_SERVER4_REQUEST_HPP
