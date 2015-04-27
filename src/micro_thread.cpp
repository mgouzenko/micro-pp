#include "micro_thread.hpp"
#include "reply.hpp"
#include "server.hpp"
#include "header.hpp"
#include "app.hpp"

namespace micro{
    void micro_thread::handle_requests(app *a){
        //Continue to handle requests until it's time to shut down. 
        while(!a->shutting_down_){
            //Grab a server from the app's queue. 
            auto s = a->q_.pop();
            if(s!=nullptr){
                //Take note of the server currently being worked on. 
                current_server_ = s; 
                //Start the stopwatch, and invoke the app's request_handler. Stop the stopwatch upon return. 
                watch.start(); 
                a->handler_(*s);
                watch.stop();
            }
        }
        //At this point, the thread has exited the while loop, and has consequently terminated. 
        status = MICRO_THREAD_TERMINATED;
    }

    void micro_thread::replace_thread(){
        //Detach the underlying thread, so that canceling it does no damage. 
        thread_.detach(); 

        //Acquire the thread's ID and kill it. 
        pthread_t id = thread_.native_handle(); 
        pthread_kill(id, SIGKILL); 

        //Reply with an "internal server error". 
        *(current_server_->reply_) = reply::stock_reply(reply::internal_server_error);
        (*current_server_)();
        watch.stop(); 
        
        //Replace the dead thread with a new one. 
        run(); 
    }

    void micro_thread::run(){
        thread_ = std::thread{[this](micro::app *a){this->handle_requests(a);}, a};
    }
}
