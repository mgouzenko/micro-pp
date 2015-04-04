#ifndef __WORK_QUEUE_HPP__
#define __WORK_QUEUE_HPP__ 

#include <queue>
#include <mutex>
#include <condition_variable> 
#include "server.hpp"

namespace micro{
	class work_queue{
		
	private:
		std::queue<micro::server> queue; 
        std::mutex mut;
        std::condition_variable cond; 
	
    public: 
        
        void push(micro::server serv){
            mut.lock(); 
            queue.push(serv);
            mut.unlock(); 
            cond.notify_one(); 
        }  

        void poke(){
            cond.notify_one();
        }

        micro::server pop(){
            std::unique_lock<std::mutex> lk(mut);

            while(queue.empty() ){
                cond.wait(lk);  
            } 

            micro::server serv = queue.front(); 
            queue.pop(); 
            lk.unlock(); 
            return serv; 
        } 
	}; 
}

#endif
