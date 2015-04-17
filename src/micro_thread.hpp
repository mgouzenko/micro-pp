#ifndef __MICRO_THREAD_HPP
#define __MICRO_THREAD_HPP

#include <thread>
#include <functional> 
#include "app.hpp"

#define MICRO_THREAD_RUNNING 0
#define MICRO_THREAD_TERMINATED 1
#define MICRO_THREAD_JOINED 2

namespace micro{

    struct app; 
    class micro_thread{

            public:
                micro_thread(micro::app *application):status{MICRO_THREAD_RUNNING}, a{application} 
                {}

                void join(){ thread_.join();  }

                micro::app* a;     

                void run(){
                    thread_ = std::thread{[this](micro::app *a){this->handle_requests(a);}, a};
                } 

                std::thread thread_;

                friend class app;

            private:
                void handle_requests(app* a); 
                
                int status; 

                
        };

}

#endif
