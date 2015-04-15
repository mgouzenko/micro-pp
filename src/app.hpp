#ifndef MICRO_APP_HPP
#define MICRO_APP_HPP

#include <boost/lockfree/queue.hpp>
#include <queue>
#include <thread>
#include "work_queue.hpp"
#include "request_handler.hpp"
#include "types.hpp"

namespace micro {

    class app {
        private:
            bool shutting_down_ = false;

            micro::request_handler handler_;    //The request handler in charge of routing requests to callbacks.

            micro::work_queue q_;

            std::vector<std::thread> thread_pool_;

            std::vector<int> thread_statuses_;

            void handle_requests(int i); //micro::request_handler& handler, boost::lockfree::queue<micro::work_item> q);

            void shut_down();

            boost::asio::io_service io_service_;

            int thread_pool_size_ = 8;

        public:

            app();

            void toggle_debug_mode(){
                handler_.set_debug_mode(); 
            }

            void run();

            void add_route(micro::url_route);

            void add_route(std::string route_specifier, micro::callback func, std::vector<std::string> methods = {"GET", "POST", "PUT", "DELETE"});

            void set_static_root(std::string static_root);

            void set_pool_size(int size) { thread_pool_size_ = size; }

            const int RUNNING = 0;

            const int TERMINATED = -1;

            const int JOINED = 1;
    };


}

#endif
