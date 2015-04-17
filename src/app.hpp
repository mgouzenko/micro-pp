#ifndef MICRO_APP_HPP
#define MICRO_APP_HPP

#include <boost/lockfree/queue.hpp>
#include <queue>
#include <thread>
#include "work_queue.hpp"
#include "request_handler.hpp"
#include "types.hpp"
#include "micro_thread.hpp"

namespace micro {

    struct micro_thread;
    class app {

        private:
            

            micro::request_handler handler_;

            micro::work_queue q_;

            std::vector<micro_thread> thread_pool_;

            std::vector<int> thread_statuses_;

            void handle_requests(int i); 

            void shut_down();

            boost::asio::io_service io_service_;

            int thread_pool_size_ = 8;

            std::string port_;

            std::string address_;

            bool shutting_down_ = false;
        
        public:
            
            friend class micro_thread; 

            app(std::string port = "8080", std::string address = "0.0.0.0");

            void toggle_debug_mode(bool toggle = true){
                handler_.set_debug_mode(toggle); 
            }

            void toggle_logging_mode(bool toggle = true){
                handler_.set_logging_mode(toggle); 
            }

            void run();

            void add_route(micro::url_route);

            void add_route(std::string route_specifier, micro::callback func, 
                    std::vector<std::string> methods = {"GET", "POST", "PUT", "DELETE"});

            void set_static_root(std::string static_root);

            void set_pool_size(int size) { thread_pool_size_ = size; }

            static const int RUNNING = 0;

            static const int TERMINATED = -1;

            static const int JOINED = 1;
    };


}

#endif
