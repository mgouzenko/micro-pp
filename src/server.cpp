#include <iostream>
#include <functional>
#include <memory>
#include <array> 

#include "server.hpp"
#include "work_queue.hpp"
#include "request.hpp"
#include "reply.hpp" 

namespace micro {

    server::server(boost::asio::io_service& io_service,
        const std::string& address,
        const std::string& port,
        micro::work_queue& q )
       : queue_{q}
    {
      tcp::resolver resolver(io_service);
      tcp::resolver::query query(address, port);
      acceptor_.reset(new tcp::acceptor(io_service, *resolver.resolve(query)));
      acceptor_->listen(1000);
	}

    #include <boost/asio/yield.hpp> // Enable the pseudo-keywords reenter, yield and fork.
    void server::operator()(boost::system::error_code ec, std::size_t length)
    {
      // In this example we keep the error handling code in one place by
      // hoisting it outside the coroutine. An alternative approach would be to
      // check the value of ec after each yield for an asynchronous operation.
      if (!ec) {
        // On reentering a coroutine, control jumps to the location of the last
        // yield or fork. The argument to the "reenter" pseudo-keyword can be a
        // pointer or reference to an object of type coroutine.
        reenter (this) {
          // Loop to accept incoming connections.
          do {
            // Create a new socket for the next incoming connection.
            socket_.reset(new tcp::socket(acceptor_->get_io_service()));

            // Accept a new connection. The "yield" pseudo-keyword saves the current
            // line number and exits the coroutine's "reenter" block. We use the
            // server coroutine as the completion handler for the async_accept
            // operation. When the asynchronous operation completes, the io_service
            // invokes the function call operator, we "reenter" the coroutine, and
            // then control resumes at the following line.
            yield acceptor_->async_accept(*socket_, *this);

            // We "fork" by cloning a new server coroutine to handle the connection.
            // After forking we have a parent coroutine and a child coroutine. Both
            // parent and child continue execution at the following line. They can
            // be distinguished using the functions coroutine::is_parent() and
            // coroutine::is_child().
            fork server(*this)();

            // The parent continues looping to accept the next incoming connection.
            // The child exits the loop and processes the connection.
          }
          while (is_parent());

          // Create the objects needed to receive a request on the connection.
          buffer_.reset(new std::array<char, 8192>);
          request_.reset(new request);

          // Loop until a complete request (or an invalid one) has been received.
          do {
            request_->client_ip = socket_->remote_endpoint().address().to_string();

            // Receive some more data. When control resumes at the following line,
            // the ec and length parameters reflect the result of the asynchronous
            // operation.
            yield socket_->async_read_some(boost::asio::buffer(*buffer_), *this);

            // Parse the data we just received.
            boost::tie(valid_request_, boost::tuples::ignore)
              = request_parser_.parse(*request_,
                  buffer_->data(), buffer_->data() + length);

            // An indeterminate result means we need more data, so keep looping.
          }
          while (boost::indeterminate(valid_request_));

          // Create the reply object that will be sent back to the client.
          reply_.reset(new reply);

          if (valid_request_) {
            // A valid request was received. Call the user-supplied function object
            // to process the request and compose a reply.
            yield queue_.push( std::shared_ptr<server>( new server(*this) ) );
          }

          else {
            // The request was invalid.
            *reply_ = reply::stock_reply(reply::bad_request);
          }

          // Send the reply back to the client.
          yield boost::asio::async_write(*socket_, reply_->to_buffers(), *this);

          // Initiate graceful connection closure.
          socket_->shutdown(tcp::socket::shutdown_both, ec);
        }
      }

      // If an error occurs then the coroutine is not reentered. Consequently, no
      // new asynchronous operations are started. This means that all shared_ptr
      // references will disappear and the resources associated with the coroutine
      // will be destroyed automatically after this function call returns.
    }

    #include <boost/asio/unyield.hpp> // Disable the pseudo-keywords reenter, yield and fork.

} // namespace micro
