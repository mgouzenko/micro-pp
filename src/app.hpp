#ifndef __MICRO_APP_HPP__
#define __MICRO_APP_HPP__

#include <thread>
#include <boost/asio.hpp>
#include "work_queue.hpp"
#include "types.hpp"
#include "request_handler.hpp"
#include "url_route.hpp"
#include "micro_thread.hpp"
#include "module.hpp" 

namespace micro {

    /**
     * **app** powers all micro++ web applications. This class encapsulates all client-server communication, including routing. 
     * **app** is built upon Boost ASIO's io_service. It uses the io_service to asynchronously receive and reply to clients' requests. 
     * When a client's request comes in, the app object matches that request to a callback function, depending on the request's URI. 
     * **app** handles requests via a thread pool. That is, when a well-formed request arrives, the associate callback function is 
     * evaluated on a separate thread. **app** also monitors its thread pool. If any thread's execution surpasses the timeout, that thread will
     * be cancelled, removed from the thread pool, and replaced. 
     */
    class app {

        private:

            friend class micro_thread;

            /***************************** Private Member Variables ************************/
            std::string port_;
            std::string address_;
            bool shutting_down_ = false;
            request_handler handler_;

            /**
             * A queue of servers to be processed in separate threads.
             */
            work_queue q_;

            /**
             * A thread to oversee the rest of the threads & make sure that they do not run longer than timeout_.
             */
            std::thread overseer_;
            int timeout_ = 3;
            std::vector<micro_thread> thread_pool_;
            int thread_pool_size_ = 8;

            /**
             * IO service that communicates asynchronously wih clients.
             */
            boost::asio::io_service io_service_;



            /***************************** Private Member Functions ************************/

            /**
             * Called by signal handler to initiate shutdown in response to SIGINT
             */
            void shut_down();

            /**
             * Function run by overseer thread to enforce timeouts on other threads.
             */
            void monitor_thread_pool();

        public:

            /**
             * Constructor for a micro++ web application.
             *
             * @param port: port number to listen for requests on.
             * @param address: address to listen for requests.
             */
            app(std::string port = "8080", std::string address = "0.0.0.0");

            /**
             * Sets the maximum time that the application is willing to wait for runaway threads
             * to finish. Any callback registered with the application will be allowed to run until
             * the specified timeout. If a thread is not finished when the timeout is up, it will be
             * killed and replaced by a new thread. In this scenario, the server will respond with
             * status code 500, internal server error. By default, the timeout is 3 seconds.
             *
             * @param time: maximum time (in seconds) a thread can run before it is terminated.
             */
            void set_timeout(int time){ timeout_ = time; }

            /**
             * Toggles debug mode. When debug mode is on, all uncaught exceptions will be displayed in
             * the browser. When debug mode is off, uncaught exceptions result in a 500, internal server error
             * response. More debug features may arrive in the future. It is recommended to turn debug
             * mode on in development, but turn it off in production. By default, debug mode is off.
             *
             * @param toggle: if true, turns debug mode on. Else turns debug mode off.
             */
            void toggle_debug_mode(bool toggle = true){
                handler_.set_debug_mode(toggle);
            }

            /**
             * Toggles logging mode. When logging mode is on, all requests are logged to stdout. When
             * logging mode is off, requests are not logged. To capture logging results in a file, it is
             * recommended for users to redirect the output of the program to a log file.
             *
             * @param toggle: if true, turns logging mode on. Else turns logging mode off.
             */
            void toggle_logging_mode(bool toggle = true){
                handler_.set_logging_mode(toggle);
            }

            /**
             * Runs the web application and blocks until SIGINT (ctrl-c) is received. It is intended
             * for run() to be called only after all url routes are bound to the application.
             */
            void run();

            //TODO: Move to private?
            void add_route(micro::url_route);

            /**
             * Registers a callback function to the specified url. When a client sends a request to
             * the specified url endpoint, using any of the permitted methods, the callback function is called. Permitted methods
             * default to GET, POST, PUT, and DELETE.
             *
             * @param route_specifier: the URL to bind the function to. See tutorial for information on capturing url parameters.
             * @param func: the callback function, to be called when a client hits the URL endpoint.
             * @param methods: the allowable methods for this specific url endpoint, and this specific callback function.
             */
            void add_route(std::string route_specifier, callback func,
                    std::vector<std::string> methods = {"GET", "POST", "PUT", "DELETE"});

            void add_module(module& mod, std::string prefix = "" );

            /**
             * Sets the static root directory - the directory in which all static files will be looked for. When processing
             * a client's request, micro++ will check the static root directory first, to see if the any static files match
             * the URI. Static files include those that never change - such as css, images, etc. By default,
             * the static root directory is ./static/
             *
             * @param static_root: either the relative, or absolute pathname to the static root directory. Relative pathnames must begin with ./ or ../
             */
            void set_static_root(std::string static_root);

            /**
             * Sets the thread pool size. When a request is received, the callback function associated with it is processed on a separate thread.
             * The thread pool size is the number of threads available to process incoming requests. By default, there are 8 threads in the thread pool.
             *
             * @param size: A positive number, indicating the number of threads to dedicate to the thread pool. If size is negative, the behavior is undefined.
             */
            void set_pool_size(int size) { thread_pool_size_ = size; }

    };


}

#endif
