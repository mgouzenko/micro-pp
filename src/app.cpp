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

    app::app() : handler_(".") { }

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

    void app::route(std::string url, micro::callback func)
    {
        handler_.route(url, func);
    }

    void app::handle_requests()
    {
        while(!shutting_down_){
            if(!q_.empty()) {
                auto serv = q_.front();
                q_.pop();
                handler_(serv);
            }
            else
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void app::shut_down() {
        shutting_down_ = true;
        for(int i = 0; i < thread_pool_.size(); i++) thread_pool_[i].join();
        io_service_.stop();
    }
}
