#ifndef __MICRO_THREAD_HPP__
#define __MICRO_THREAD_HPP__

#include <thread>
#include "stop_watch.hpp" 

#define MICRO_THREAD_RUNNING 0
#define MICRO_THREAD_TERMINATED 1
#define MICRO_THREAD_JOINED 2

namespace micro{

    struct server;
    struct app;

    /**
     * This is a wrapper around **std::thread** that additionally provides the ease to monitor thread status, 
     * keep track of the current server being worked on, and replace timed out threads. **micro_thread** performs
     * all of the work to invoke the appropriate callback function on behalf of the application. The crux of **micro_thread**
     * is that it keeps track of a particular instance of **app**. **micro_thread** acts as
     * a "consumer" by popping servers off the **app**'s queue, and invoking the correct callback function. The **app** acts as a bridge
     * between the asynchronous server (which produces items for the queue), and a thread pool of **micro_threads**. 
     */
    class micro_thread{

        friend class app;

        /**
         * Constructor for **micro_thread**. 
         * @param application: the **app** this thread is associated with. 
         */
        micro_thread(micro::app *application):
            status{MICRO_THREAD_RUNNING}, a{application}, current_server_{nullptr}
        {}

        micro::app* a;

        /**
         * The underlying thread of execution. 
         */
        std::thread thread_;

        /**
         * The status of the thread, initially set to MICRO_THREAD_RUNNING. Other statuses are #define'd above. 
         */
        int status;

        /**
         * Current server coroutine being worked on. Really, this is just a collection of pointers to a socket, an acceptor, 
         * a request, and a response. 
         */
        std::shared_ptr<server> current_server_;

        /**
         * The **stop_watch** that the thread uses to time execution of callbacks. 
         */
        stop_watch watch;

        /**
         * Spawns a new thread of execution. 
         */
        void run();

        /**
         * Called to replace threads that have exceeded timeout. 
         */
        void replace_thread();

        void join(){ thread_.join();  }

        /**
         * The function that performs all the work this thread does to handle incoming requests on behalf of the underlying **app**.
         * @param a: the **app** for which this thread shall handle requests. 
         */
        void handle_requests(app* a);
    };
}

#endif
