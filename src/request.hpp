#ifndef MICRO_REQUEST_HPP
#define MICRO_REQUEST_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "header.hpp"

namespace micro {

    /**
     * A request received from a client.
     */
    struct request
    {
      /**
       * The request method, e.g. "GET", "POST".
       */
      std::string method;

      /**
       * The requested URI, such as a path to a file.
       */
      std::string uri;
    
      /**
       * The ip address of the client. 
       */
      std::string client_ip; 

      /**
       * Major version number, usually 1.
       */
      int http_version_major;

      /**
       * Minor version number, usually 0 or 1.
       */
      int http_version_minor;

      /**
       * The headers included with the request.
       */
      std::vector<header> headers;

      /**
       * The optional content sent with the request.
       */
      std::string content;

      /**
       * Fields populated after the URL is matched:
       */
      std::unordered_map<std::string, std::string> label_values;

      /**
      * Map of cookies as key value pairs
      */
      std::unordered_map<std::string, std::string> cookies;

      /**
      * Hostname from HTTP Host header
      */
      std::string hostname;

      /**
      * Get the value of a cookie. Returns empty string if not found
      * @param key: key value of the cookie
      */
      std::string get_cookie(const std::string& key);

      /**
      * Get the hostname from the HTTP Host header
      */
      const std::string& get_hostname() const;

    };

} // namespace micro

#endif // MICRO_REQUEST_HPP
