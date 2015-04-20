#ifndef __STOP_WATCH_HPP__
#define __STOP_WATCH_HPP__
#include <ctime>

namespace micro{

    class stop_watch{

        private:
            std::time_t last_recorded_time_;

            bool is_running_ = false; 

        public:

            stop_watch();

            int get_time();

            void start();

            void stop();
    };
}

#endif
