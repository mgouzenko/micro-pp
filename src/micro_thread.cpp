#include "micro_thread.hpp"
#include "app.hpp"

namespace micro{
        void micro_thread::handle_requests(app *a){
            while(!a->shutting_down_){
                auto s = a->q_.pop();
                if(s!=nullptr){
                    current_server_ = &*s; 
                    watch.start(); 
                    a->handler_(*s);
                    watch.stop();
                }
            }
            status = MICRO_THREAD_TERMINATED;
        }
}
