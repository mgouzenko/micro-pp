#ifndef __WORK_QUEUE_HPP__
#define __WORK_QUEUE_HPP__ 

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable> 

namespace micro{
    struct server; 
    class work_queue{
		
	private:
		std::queue< std::shared_ptr<server> > queue; 
        bool shutting_down_ = false; 

    public: 
        std::mutex mut;
        
        std::condition_variable cond; 

        void prepare_for_shutdown(){ shutting_down_ = true; } 

        void push(std::shared_ptr<server> serv);

        void poke();
        
        std::shared_ptr<server> pop();
    
    }; 
}

#endif
