#include "micro_thread.hpp"
#include "reply.hpp"
#include "server.hpp"
#include "header.hpp"
#include "app.hpp"

namespace micro{
    void micro_thread::handle_requests(app *a){
        while(!a->shutting_down_){
            auto s = a->q_.pop();
            if(s!=nullptr){
                current_server_ = s; 
                watch.start(); 
                a->handler_(*s);
                watch.stop();
            }
        }
        status = MICRO_THREAD_TERMINATED;
    }

    void micro_thread::replace_thread(){
        thread_.detach(); 
        pthread_t id = thread_.native_handle(); 
        pthread_kill(id, SIGKILL); 

        *(current_server_->reply_) = reply::stock_reply(reply::internal_server_error);
        (*current_server_)();
        watch.stop(); 
        run(); 
    }

    void micro_thread::run(){
        thread_ = std::thread{[this](micro::app *a){this->handle_requests(a);}, a};
    }
}
