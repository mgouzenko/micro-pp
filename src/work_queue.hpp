#ifndef __WORK_QUEUE_HPP__
#define __WORK_QUEUE_HPP__ 

#include <queue>
#include <mutex>
#include <condition_variable> 
#include "server.hpp"
#include "request_handler.hpp"
#include <memory>

namespace micro{
	class work_queue{
		
	private:
		std::queue< std::shared_ptr<micro::server> > queue; 
        bool shutting_down_ = false; 

    public: 
        std::mutex mut;
        
        std::condition_variable cond; 

        void prepare_for_shutdown(){ shutting_down_ = true; } 

        void push(std::shared_ptr<micro::server> serv){
            mut.lock(); 
            queue.push(serv);
            mut.unlock(); 
            cond.notify_one(); 
        }  

        void poke(){
            cond.notify_one();
        }

        std::shared_ptr<micro::server> pop(){
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
	}; 
}

#endif
