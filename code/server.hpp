//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER4_SERVER_HPP
#define HTTP_SERVER4_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <unordered_map>
#include <boost/asio/coroutine.hpp>
#include "request_parser.hpp"
#include <functional>
#include "types.hpp"
#include <boost/lockfree/queue.hpp>
#include <utility> 
#include <memory>
#include <queue>

namespace http {
namespace server4 {

struct request;
struct reply;

/// The top-level coroutine of the HTTP server.
class server : boost::asio::coroutine
{

private:
  typedef boost::asio::ip::tcp tcp;

public:
  /// Construct the server to listen on the specified TCP address and port, and
  /// serve up files from the given directory.
  explicit server(boost::asio::io_service& io_service,
      			  const std::string& address, 
				  const std::string& port,
				  std::queue<server>& q
	  			);

  /// Perform work associated with the server.
  void operator()(
      boost::system::error_code ec = boost::system::error_code(),
      std::size_t length = 0);

  void route(std::string url, Callback);

  /// The work queue
  std::queue<server>& queue; 

  /// Acceptor used to listen for incoming connections.
  boost::shared_ptr<tcp::acceptor> acceptor_;

  /// The current connection from a client.
  boost::shared_ptr<tcp::socket> socket_;

  /// Buffer for incoming data.
  boost::shared_ptr<boost::array<char, 8192> > buffer_;

  /// The incoming request.
  boost::shared_ptr<request> request_;

  /// Whether the request is valid or not.
  boost::tribool valid_request_;

  /// The parser for the incoming request.
  request_parser request_parser_;

  /// The reply to be sent back to the client.
  boost::shared_ptr<reply> reply_;
};

} // namespace server4
} // namespace http

#endif // HTTP_SERVER4_SERVER_HPP
