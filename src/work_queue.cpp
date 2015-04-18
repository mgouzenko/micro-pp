#include "work_queue.hpp"

namespace micro{    

    void work_queue::push(std::shared_ptr<server> serv){
        mut.lock(); 
        queue.push(serv);
        mut.unlock(); 
        cond.notify_one(); 
    }   

    void work_queue::poke(){
        cond.notify_one();
    }   

    std::shared_ptr<server> work_queue::pop(){
        std::unique_lock<std::mutex> lk(mut);
        while(queue.empty() ){
            if(shutting_down_){ 
                lk.unlock();
                return NULL;
            }   
            cond.wait(lk);  
        }   

        auto serv = queue.front(); 
        queue.pop(); 
        lk.unlock(); 

        return serv;
    } 
}
