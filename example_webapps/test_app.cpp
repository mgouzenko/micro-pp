
#include "app.hpp"
#include "request.hpp" 


std::string hello(const http::server4::request&){
	return "hello world!"; 
}


int main(int argc, char** argv){

	micro::app application; 
	application.route("/hello", hello);
	application.run();

} 
