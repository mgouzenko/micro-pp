#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <signal.h>
#include <thread>
#include <chrono>

#include "app.hpp"
#include "server.hpp"
#include "file_handler.hpp"
#include "request_handler.hpp"

namespace micro {

    app::app() : handler_() { }

    void app::run()
    {

      try {

        for(int i=0; i<1; i++) {
            std::cout << "added worker\n";
            thread_pool_.push_back(std::thread(&app::handle_requests, this));
        }

        //for(int i=0; i<workers.size(); i++) workers[i].detach();

        micro::server(io_service_, "0.0.0.0", "8080", q_)();
        // Wait for signals indicating time to shut down.
        boost::asio::signal_set signals(io_service_);
        signals.add(SIGINT);
        signals.add(SIGTERM);
        #if defined(SIGQUIT)
        signals.add(SIGQUIT);
        #endif // defined(SIGQUIT)

        signals.async_wait(boost::bind(
              &app::shut_down, this));

        //signals.async_wait(boost::bind(
        //    &boost::asio::io_service::stop, &io_service));

        // Run the server.
        io_service_.run();
      }
      catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
      }

    }

    void app::add_route(micro::url_route route)
    {
        handler_.add_route(route);
    }

    void app::add_route(std::string route_specifier, std::vector<std::string> methods, micro::callback func)
    {
        add_route(micro::url_route(route_specifier, methods, func));
    }

    void app::set_static_root(std::string static_root)
    {
        handler_.set_static_root(static_root);
    }

    void app::handle_requests()
    {
        while(!shutting_down_){
            auto serv = q_.pop();
            q_.pop();
            handler_(serv);
        }
    }

    void app::shut_down() {
        shutting_down_ = true;
        for(int i = 0; i < thread_pool_.size(); i++) thread_pool_[i].join();
        io_service_.stop();
    }
}
