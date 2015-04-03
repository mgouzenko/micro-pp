#ifndef MICRO_APP_HPP
#define MICRO_APP_HPP

#include <boost/lockfree/queue.hpp>
#include <queue>
#include <thread>

#include "request_handler.hpp"
#include "types.hpp"

namespace micro {

    class app {
        private:
            bool shutting_down_ = false;

            micro::request_handler handler_;    //The request handler in charge of routing requests to callbacks.

            std::queue<micro::server> q_;

            std::vector<std::thread> thread_pool_;

            void handle_requests();//micro::request_handler& handler, boost::lockfree::queue<micro::work_item> q);

            void shut_down();

            boost::asio::io_service io_service_;

        public:
            app();

            void run();

            void add_route(micro::url_route);

            void add_route(std::string route_specifier, std::vector<std::string> methods, micro::callback func);

            void set_static_root(std::string static_root);
    };


}

#endif
