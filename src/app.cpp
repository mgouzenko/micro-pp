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
#include "module.hpp"

namespace micro {

    app::app(std::string port, std::string address): 
        handler_(), port_{port}, address_{address} 
    {}

    void app::run()
    {
        try {

            /* Create a thread pool */
            for(int i=0; i<thread_pool_size_; i++){
                thread_pool_.push_back(micro_thread(this) );
            }

            /* Start the threads. */
            for(int i=0; i<thread_pool_size_; i++){
                thread_pool_[i].run();
            }
           
            /* Start the overseer_ thread, which will enforce timeout_ on other threads.*/
            overseer_ = std::thread{ [this](){ this->monitor_thread_pool(); } }; 
        
            /* Create an server on the specified port and address, and bind it to the io_service. */
            server(io_service_, address_, port_, q_)();

            /* Wait for signals indicating time to shut down. */
            boost::asio::signal_set signals(io_service_);
            signals.add(SIGINT);
            signals.add(SIGTERM);
        
            #if defined(SIGQUIT)
                signals.add(SIGQUIT);
            #endif

            signals.async_wait(boost::bind(&app::shut_down, this));

            /* Run the io_service to start the webapp. */
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

    void app::add_module(module& m, std::string prefix){
        auto b_itr = m.begin(); 
        auto e_itr = m.end(); 

        while(b_itr != e_itr){
            add_route( prefix + (*b_itr).route_, (*b_itr).callback_, (*b_itr).methods_ );
            ++b_itr;
        }
    }

    void app::set_static_root(std::string static_root)
    {
        handler_.set_static_root(static_root);
    }
    
    void app::monitor_thread_pool(){
        int num_threads = thread_pool_.size(); 
        // Loop until shutdown
        while(!shutting_down_){ 
            // Sleep so as to only check for timeouts periodically. 
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
            
            // Iterate over all threads 
            for(int i = 0; i < num_threads; i++){
                // Get the time elapsed since the callback began
                int seconds = thread_pool_[i].watch.get_time();
                
                //If the thread has been running too long, replace it.
                if(seconds > timeout_){
                    pthread_t id = thread_pool_[i].thread_.native_handle(); 
                    BOOST_LOG_TRIVIAL(warning) << "Timeout! Canceling thread: " << id;
                    thread_pool_[i].replace_thread(); 
                }
            }
        }
    
    }
 
    void app::shut_down() {
        try{
            shutting_down_ = true;
            // Prepare the queue for shutdown, making sure threads will no longer wait for jobs. 
            q_.prepare_for_shutdown();

            int num_threads = thread_pool_.size();
            int joined_threads = 0;
            
            for(;;){
                // Continue to poke the queue to make sure threads aren't sleeping. 
                for(int i = 0; i < num_threads - joined_threads; i++) q_.poke();

                // Iterate over all threads.
                for(int i = 0; i < num_threads; i++){
                    //If the thread has been terminated, join it and set its status to JOINED. 
                    if(thread_pool_[i].status == MICRO_THREAD_TERMINATED){
                        thread_pool_[i].join();
                        thread_pool_[i].status = MICRO_THREAD_JOINED;
                        //If we've joined ALL of the threads, stop the io_service, terminate the overseer_, and exit. 
                        if(++joined_threads == num_threads) {
                            io_service_.stop();
                            overseer_.join();
                            return;
                        }
                    }
                }
            } 
        } catch(std::exception& e) { std::cout << e.what(); }
    }

}
