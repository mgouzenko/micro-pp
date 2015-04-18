#include "stop_watch.hpp"

namespace micro{
    
    stop_watch::stop_watch(){
        time(&last_recorded_time_); 
    }

    int stop_watch::get_time(){
        if(!is_running_) return 0; 
        std::time_t new_time; 
        time(&new_time); 
        return difftime(new_time, last_recorded_time_);
    } 

    void stop_watch::start(){
        is_running_ = true; 
        time(&last_recorded_time_); 
    }

    void stop_watch::stop(){
        is_running_ = false; 
    }

}
