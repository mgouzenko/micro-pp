#include "request_handler.hpp"
#include "types.hpp" 
#include <boost/lockfree/queue.hpp> 
#include <queue>

namespace micro{	
	
	class app{
		private:
			http::server4::request_handler	handler;    //The request handler in charge of routing requests to callbacks. 
		
			std::queue<http::server4::server> q;	

			void handle_requests();//http::server4::request_handler& handler, boost::lockfree::queue<http::server4::work_item> q);
		
		public:
			app(); 

			void run(); 

			void route(std::string url, Callback func); 
	};


}
