#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <signal.h>
#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>

#include "app.hpp"
#include "url_route.hpp"
#include "server.hpp" 
#include "micro_thread.hpp"

namespace micro {

    app::app(std::string port, std::string address): 
        handler_(), port_{port}, address_{address} 
    {}

    void app::run()
    {
        try {
            for(int i=0; i<thread_pool_size_; i++){
                thread_pool_.push_back(micro_thread(this) );
            }

            for(int i=0; i<thread_pool_size_; i++){
                thread_pool_[i].run();
            }
            
            overseer_ = std::thread{ [this](){ this->monitor_thread_pool(); } }; 
        

            server(io_service_, address_, port_, q_)();
            // Wait for signals indicating time to shut down.
            boost::asio::signal_set signals(io_service_);
            signals.add(SIGINT);
            signals.add(SIGTERM);
        
            #if defined(SIGQUIT)
                signals.add(SIGQUIT);
            #endif

            signals.async_wait(boost::bind(&app::shut_down, this));

            io_service_.run();
        }
        catch (std::exception& e) {
            std::cerr << "exception: " << e.what() << "\n";
        }
    }

    void app::add_route(url_route route)
    {
        handler_.add_route(route);
    }

    void app::add_route(std::string route_specifier, callback func, std::vector<std::string> methods)
    {
        add_route(micro::url_route(route_specifier, func, methods));
    }

    void app::set_static_root(std::string static_root)
    {
        handler_.set_static_root(static_root);
    }
    
    void app::monitor_thread_pool(){
        int num_threads = thread_pool_.size(); 
        while(!shutting_down_){ 
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
            for(int i = 0; i < num_threads; i++){
                int seconds = thread_pool_[i].watch.get_time(); 
                if(seconds > timeout_){
                    BOOST_LOG_TRIVIAL(warning) << "Thread Timeout!"; 
                    replace_thread(i); 
                }
            }
        }
    
    }

    void app::replace_thread(int i){
        pthread_t id = thread_pool_[i].thread_.native_handle(); 
        BOOST_LOG_TRIVIAL(warning) << "Canceling thread: " << id;
        thread_pool_[i].replace_thread(); 
    } 
    
    void app::shut_down() {
        try{
            shutting_down_ = true;
            q_.prepare_for_shutdown();

            int num_threads = thread_pool_.size();
            int joined_threads = 0;
            overseer_.join();
            for(;;){
                for(int i = 0; i < num_threads - joined_threads; i++) q_.poke();
                for(int i = 0; i < num_threads; i++){
                    if(thread_pool_[i].status == MICRO_THREAD_TERMINATED){
                        thread_pool_[i].join();
                        thread_pool_[i].status = MICRO_THREAD_JOINED;
                        if(++joined_threads == num_threads) {
                            io_service_.stop();
                            return;
                        }
                    }
                }
            }
        } catch(std::exception& e) { std::cout << e.what(); }
    }
}
