
#include "app.hpp"
#include "request.hpp" 
#include "reply.hpp"


void hello(const http::server4::request& req, http::server4::reply& rep){
	rep.render_string("hello world!"); 
  rep.set_cookie("first", "1");
  rep.set_cookie("second", "2");
}


int main(int argc, char** argv){

	micro::app application; 
	application.route("/hello", hello);
	application.run();

} 
