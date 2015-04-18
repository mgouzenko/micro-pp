#ifndef MICRO_APP_HPP
#define MICRO_APP_HPP
#include <thread>

#include <boost/asio.hpp> 
#include "work_queue.hpp"
#include "types.hpp" 
#include "request_handler.hpp" 
#include "url_route.hpp"    
#include "micro_thread.hpp" 

namespace micro {

    class app {

        private:
            
            request_handler handler_;

            work_queue q_;

            std::vector<micro_thread> thread_pool_;

            std::vector<int> thread_statuses_;

            void handle_requests(int i); 

            void shut_down();

            boost::asio::io_service io_service_;

            int thread_pool_size_ = 8;

            std::string port_;

            std::string address_;

            bool shutting_down_ = false;
       
            void monitor_thread_pool(); 
            
            void replace_thread(int i);

            std::thread overseer_; 

            int timeout_ = 3; 

        public:
            
            friend class micro_thread; 

            app(std::string port = "8080", std::string address = "0.0.0.0");

            void set_timeout(int time){ timeout_ = time; }

            void toggle_debug_mode(bool toggle = true){
                handler_.set_debug_mode(toggle); 
            }

            void toggle_logging_mode(bool toggle = true){
                handler_.set_logging_mode(toggle); 
            }

            void run();

            void add_route(micro::url_route);

            void add_route(std::string route_specifier, callback func, 
                    std::vector<std::string> methods = {"GET", "POST", "PUT", "DELETE"});

            void set_static_root(std::string static_root);

            void set_pool_size(int size) { thread_pool_size_ = size; }

            static const int RUNNING = 0;

            static const int TERMINATED = -1;

            static const int JOINED = 1;
    };


}

#endif
