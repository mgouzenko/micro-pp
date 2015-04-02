#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <signal.h>
#include "server.hpp"
#include "file_handler.hpp"
#include "app.hpp"
#include <thread>
#include "request_handler.hpp"
#include <chrono>

namespace micro {

    app::app() : handler(".") { }

    void app::run()
    {
      try {

        for(int i=0; i<1; i++) {
            std::cout << "added worker\n";
            thread_pool.push_back(std::thread(&app::handle_requests, this));
        }

        //for(int i=0; i<workers.size(); i++) workers[i].detach();

        http::server4::server(io_service, "0.0.0.0", "8080", q)();
        // Wait for signals indicating time to shut down.
        boost::asio::signal_set signals(io_service);
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
        io_service.run();
      }
      catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
      }

    }

    void app::route(std::string url, micro::callback func)
    {
        handler.route(url, func);
    }

    void app::handle_requests()
    {
        while(!shutting_down){
            if(!q.empty()) {
                auto serv = q.front();
                q.pop();
                handler(serv);
            }
            else
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void app::shut_down() {
        shutting_down = true;
        for(int i = 0; i < thread_pool.size(); i++) thread_pool[i].join();
        io_service.stop();
    }
}
