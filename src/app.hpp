#include "request_handler.hpp"
#include "types.hpp"
#include <boost/lockfree/queue.hpp>
#include <queue>
#include <thread>

namespace micro {

    class app {
        private:
            bool shutting_down = false;

            micro::request_handler  handler;    //The request handler in charge of routing requests to callbacks.

            std::queue<micro::server> q;

            std::vector<std::thread> thread_pool;

            void handle_requests();//micro::request_handler& handler, boost::lockfree::queue<micro::work_item> q);

            void shut_down();

            boost::asio::io_service io_service;

        public:
            app();

            void run();

            void route(std::string url, micro::callback func);
    };


}
