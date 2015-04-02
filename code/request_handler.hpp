//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include <string>
#include <unordered_map> 
#include <functional> 
#include "types.hpp"
#include "server.hpp"
#include "response.hpp"

namespace http {
namespace server4 {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler
{
public:
  /// Construct with a directory containing files to be served.
  explicit request_handler(const std::string& doc_root);

  /// Handle a request and produce a reply.
  void operator()(server& serv);

  void route(std::string url, Callback func); 

private:
  /// The directory containing the files to be served.
  std::string doc_root_;

  /// The map of urls to Callback functions 
  std::unordered_map<std::string, Callback > callback_urls;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);

  response response_;

};

} // namespace server4
} // namespace http

#endif // HTTP_SERVER4_FILE_HANDLER_HPP
