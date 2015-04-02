#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <signal.h>
#include "server.hpp"
#include "request_handler.hpp"


int main(int argc, char* argv[])
{
  try {
    // Check command line arguments.
    if (argc != 4) {
      std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 80 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 80 .\n";
      return 1;
    }

    boost::asio::io_service io_service;

    // Launch the initial server coroutine.
    //http::server4::server(io_service, argv[1], argv[2], http::server4::file_handler(argv[3]))();

	//std::unordered_map<std::string, Callback > urls;
	http::server4::request_handler handler(".");
	http::server4::server(io_service, "0.0.0.0", "8080", handler)();

	// Wait for signals indicating time to shut down.
    boost::asio::signal_set signals(io_service);
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    signals.async_wait(boost::bind(
          &boost::asio::io_service::stop, &io_service));

    // Run the server.
    io_service.run();
  }
  catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
