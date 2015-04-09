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

        for(int i=0; i<thread_pool_size_; i++) {
            std::cout << "added worker\n";
            thread_statuses_.push_back(RUNNING);
            thread_pool_.push_back(std::thread([this, i](){this->handle_requests(i); }  ));
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

    void app::add_route(std::string route_specifier, micro::callback func, std::vector<std::string> methods)
    {
        add_route(micro::url_route(route_specifier, func, methods));
    }

    void app::set_static_root(std::string static_root)
    {
        handler_.set_static_root(static_root);
    }

    void app::handle_requests(int i)
    {
        while(!shutting_down_){
            q_.do_work(handler_);
        }
        std::cout << "ended\n";
        thread_statuses_[i] = TERMINATED;
    }

    void app::shut_down() {
        shutting_down_ = true;
        q_.prepare_for_shutdown();

        int num_threads = thread_pool_.size();
        int joined_threads = 0;
        for(;;){
            for(int i = 0; i < num_threads - joined_threads; i++) q_.poke();
            for(int i = 0; i < num_threads; i++){
                if(thread_statuses_[i] == TERMINATED){
                    thread_pool_[i].join();
                    thread_statuses_[i] = JOINED;
                    if(++joined_threads == num_threads) {
                        io_service_.stop();
                        return;
                    }
                }
            }
        }
    }
}
