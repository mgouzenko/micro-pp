#include "request_handler.hpp"
#include "types.hpp" 

namespace micro{	
	
	class app{
		private:
			http::server4::request_handler	handler;    //The request handler in charge of routing requests to callbacks. 
	
	
		public:
			app(); 

			void run(); 

			void route(std::string url, Callback func); 
	};


}
