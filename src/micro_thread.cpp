#include "micro_thread.hpp"
#include "app.hpp"

namespace micro{
        void micro_thread::handle_requests(app *a){
            while(!a->shutting_down_){
                auto s = a->q_.pop();
                if(s!=nullptr) a->handler_(*s); 
            }
            status = MICRO_THREAD_TERMINATED;
        }
}
