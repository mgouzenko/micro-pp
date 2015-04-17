#ifndef __STOP_WATCH_HPP__
#define __STOP_WATCH_HPP__
#include <ctime> 

namespace micro{

    class stop_watch{

        private: 
            std::time_t last_recorded_time_;  
            
            bool is_running_; 

        public:
            stop_watch(){
                time(&last_recorded_time_); 
            }

            int get_time(){
                if(!is_running_) return 0; 
                std::time_t new_time; 
                time(&new_time); 
                return difftime(new_time, last_recorded_time_);
            } 

            void start(){
                is_running_ = true; 
                time(&last_recorded_time_); 
            }

            void stop(){
                is_running_ = false; 
            }

    };
}

#endif
