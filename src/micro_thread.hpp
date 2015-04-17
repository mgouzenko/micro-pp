#ifndef __MICRO_THREAD_HPP
#define __MICRO_THREAD_HPP

#include <thread>
#include <functional> 
#include "app.hpp"
#include "stop_watch.hpp"
#include "server.hpp"

#define MICRO_THREAD_RUNNING 0
#define MICRO_THREAD_TERMINATED 1
#define MICRO_THREAD_JOINED 2

namespace micro{

    struct app; 
    class micro_thread{
                
        friend class app;        

        micro_thread(micro::app *application):status{MICRO_THREAD_RUNNING}, a{application} 
        {}

        micro::app* a;     

        std::thread thread_;
 
        int status; 

        server* current_server_; 

        stop_watch watch; 

        void run(){
            thread_ = std::thread{[this](micro::app *a){this->handle_requests(a);}, a};
        }

        void replace_thread(){
            thread_.detach(); 
            pthread_t id = thread_.native_handle(); 
            current_server_->socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both); 
            pthread_kill(id, SIGKILL); 
           
            watch.stop(); 
            run(); 
        }

        void join(){ thread_.join();  } 

        void handle_requests(app* a); 
    
    };
}

#endif
