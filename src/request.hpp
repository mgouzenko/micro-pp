#ifndef __MICRO_REQUEST_HPP__
#define __MICRO_REQUEST_HPP__

#include <string>
#include <vector>
#include <unordered_map>

#include "header.hpp"
namespace micro {
    /**
     * The **request** object provides an API that allows developers access to data from a HTTP request. 
     * The **requst** object can be thought of as a wrapper for a HTTP request. The web-framework parses 
     * the HTTP request and populates the appropriate data in the request object. 
     */
    class request
    {
      friend class request_handler;
      friend class request_parser;
      friend class server;
      friend class url_route; 
      /**
       * The HTTP requst method: e.g. "GET", "POST", "POST", "DELETE".
       */
      std::string method;

      /**
       * The URI defined in the HTTP request. HTTP performs method on the defined URI.
       */
      std::string uri;

      /**
       * The ip address of the client.
       */
      std::string client_ip;

      /**
       * The major version number for HTTP protocol.
       */
      int http_version_major;

      /**
       * The minor version number for the HTTP protocol.
       */
      int http_version_minor;

      /**
       * Each line of the request headers is mapped to a a vector of headers. 
       */
      std::vector<header> headers;

      /**
       * The content body of the HTTP request. Post params are stored in the content body. 
       */
      std::string content;

      /**
       * The dynamic url parameters stored in an unordered_map.
       */
      std::unordered_map<std::string, std::string> label_values;

      /**
      * Cookies stored in an unordered map.
      */
      std::unordered_map<std::string, std::string> cookies_;

      /**
      * Post parameters stored in an unordered_map. Parsed from content body.
      */
      std::unordered_map<std::string, std::string> post_params_;

      /**
       * Get parameters stored in an unordered_map. Parsed from query string.
       */
      std::unordered_map<std::string, std::string> get_params_;

      /**
      * Hostname provided by the HTTP Host header.
      */
      std::string hostname_;


public:
      /**
      * Cookies are stored in key, value pairs.
      * Search for a cookie given a key as a string.
      * Returns an empty string if no cookie is availble for the given key.
      * @param key: key value of the cookie
      */
      std::string get_cookie(const std::string& key) const;

      /**
      * POST parameters are stored in key, value pairs.
      * Search for a POST parameter issued by a client.
      * Returns empty string if no get parameter is available for the given key.
      * @param key: key value of the post param
      */
      std::string get_post_param(const std::string& key) const;

      /**
      * GET parameters are stored in key, value pairs.
      * Search for a get paramter parsed from the querystring.
      * Returns empty string if not found
      * (eg. ?key=value)
      * @param key: key value of the GET param
      */
      std::string get_query_param(const std::string& key) const;

      /**
      * Dynamic url route parameters are stored in key, value pairs
      * Access to dynamic url paramters are stored by the key registered
      * in the route.
      * Returns empty string if not found
      * (eg. /api/<user>)
      * @param key: key value of the url param
      */
      std::string get_route_param(const std::string& key) const;

      /**
      * Get the hostname from the HTTP Host header
      */
      const std::string& get_hostname() const;

      /**
      * Get the URI of the request. The URI represents the path that the 
      * path for which the client issued the HTTP request.
      */
      const std::string& get_uri() const;

      /**
      * Get the ip address from the client that issued the request. 
      * (eg. 160.39.251.11)
      */
      const std::string& get_ip() const;

      /**
      * Get the method of the HTTP request. 
      * (eg. GET, POST, PUT, DELETE)
      */
      const std::string& get_method() const;

    };

} // namespace micro

#endif // MICRO_REQUEST_HPP
