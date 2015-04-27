#ifndef __MICRO_WORK_QUEUE_HPP__
#define __MICRO_WORK_QUEUE_HPP__

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace micro{
    struct server;
    
    /**
     * This is the queue used by the webserver and thread pool to enqueue and process requests, respectively. 
     * When the server finishes asynchronously building a complete request, it will enqueue itself on the 
     * **work_queue**. Since the server is a coroutine that consists of nothing more than a few pointers, 
     * it can enqueue itself without much overhead. On the other ends, threads from the thread pool dequeue 
     * servers from the **work_queue** and invoke the appropriate callback function on the completed request. 
     * In this model, the server is the producer, and the thread pool threads are the consumers. 
     */
    class work_queue{
		
    friend class app; 
    friend class micro_thread; 
    friend class server; 

    /**
     * The actual queue. 
     */
    std::queue< std::shared_ptr<server> > queue; 
    
    /**
     * Flag indicating whether or not it's time to shut down. 
     */
    bool shutting_down_ = false; 

    /**
     * Mutex to synchronize producer/consumer model. 
     */
    std::mutex mut;
    
    /**
     * Condition variable to notify any threads waiting on an empty queue. 
     */
    std::condition_variable cond; 

    /**
     * This function is invoked by **app** when it's time to shut down the application. 
     * It causes the threads waiting on an empty queue to stop trying to wait for a new job. 
     */
    void prepare_for_shutdown(){ shutting_down_ = true; } 

    /**
     * Thread safe push.
     * @param serv: the server coroutine to be enqueued. 
     */
    void push(std::shared_ptr<server> serv);

    /**
     * Guaranteed to wake up a thread waiting for the queue to be non-empty. Used in shutdown.  
     */
    void poke();
    
    /**
     * Thread safe pop. 
     */
    std::shared_ptr<server> pop();
    
    }; 
}

#endif
