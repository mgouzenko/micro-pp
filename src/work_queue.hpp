#ifndef __WORK_QUEUE_HPP__
#define __WORK_QUEUE_HPP__ 

#include <queue>
#include <mutex>
#include <condition_variable> 
#include "server.hpp"
#include "request_handler.hpp"

namespace micro{
	class work_queue{
		
	private:
		std::queue<micro::server> queue; 
        std::mutex mut;
        std::condition_variable cond; 
        bool shutting_down_ = false; 

    public: 
        
        void prepare_for_shutdown(){ shutting_down_ = true; } 

        void push(micro::server serv){
            mut.lock(); 
            queue.push(serv);
            mut.unlock(); 
            cond.notify_one(); 
        }  

        void poke(){
            cond.notify_one();
        }

        void do_work(micro::request_handler handler){
            std::unique_lock<std::mutex> lk(mut);

            while(queue.empty() ){
                if(shutting_down_) return;
                cond.wait(lk);  
            } 

            micro::server serv = queue.front(); 
            queue.pop(); 
            lk.unlock(); 
            handler(serv); 
        } 
	}; 
}

#endif
