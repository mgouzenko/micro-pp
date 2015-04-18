#ifndef __MICRO_THREAD_HPP
#define __MICRO_THREAD_HPP

#include <thread>
#include <functional> 

#include "stop_watch.hpp" 

#define MICRO_THREAD_RUNNING 0
#define MICRO_THREAD_TERMINATED 1
#define MICRO_THREAD_JOINED 2

namespace micro{
    
    struct server; 
    struct app; 
    class micro_thread{
                
        friend class app;        

        micro_thread(micro::app *application):
            status{MICRO_THREAD_RUNNING}, a{application}, current_server_{nullptr} 
        {}

        micro::app* a;     

        std::thread thread_;
 
        int status; 

        std::shared_ptr<server> current_server_; 

        stop_watch watch; 

        void run();

        void replace_thread(); 

        void join(){ thread_.join();  } 

        void handle_requests(app* a); 
    
    };
}

#endif
