#include "request_handler.hpp"
#include "types.hpp"
#include <boost/lockfree/queue.hpp>
#include <queue>
#include <thread>

namespace micro {

    class app {
        private:
            bool shutting_down = false;

            http::server4::request_handler  handler;    //The request handler in charge of routing requests to callbacks.

            std::queue<http::server4::server> q;

            std::vector<std::thread> thread_pool;

            void handle_requests();//http::server4::request_handler& handler, boost::lockfree::queue<http::server4::work_item> q);

            void shut_down();

            boost::asio::io_service io_service;

        public:
            app();

            void run();

            void route(std::string url, micro::callback func);
    };


}
