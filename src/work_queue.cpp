#include "work_queue.hpp"

namespace micro{    

    void work_queue::push(std::shared_ptr<server> serv){
        //Lock the mutex, push the server, and unlock immediately. 
        mut.lock(); 
        queue.push(serv);
        mut.unlock(); 
        
        //Notify possibly waiting threads using the condition variable. 
        cond.notify_one(); 
    }   

    void work_queue::poke(){
        cond.notify_one();
    }   

    std::shared_ptr<server> work_queue::pop(){
        //Establish a unique lock on the mutex. 
        std::unique_lock<std::mutex> lk(mut);
        
        while(queue.empty() ){
            if(shutting_down_){ 
                lk.unlock();
                return NULL;
            } 

            //Sleep on the empty queue if we're not shutting down. 
            cond.wait(lk);  
        }   

        //If we've gotten this far, we have the lock, and the server isn't empty. 
        auto serv = queue.front(); 
        queue.pop(); 
        lk.unlock(); 

        return serv;
    } 
}
