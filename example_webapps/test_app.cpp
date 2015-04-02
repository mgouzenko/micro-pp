
#include "app.hpp"
#include "request.hpp" 
#include "response.hpp"
#include "cookie.hpp"
#include <ctime>


void hello(const http::server4::request& req, http::server4::response& res){
  time_t now = time(0) + 100; 
  res.render_string("hello world"); 
  http::server4::Cookie c = http::server4::Cookie("fourth", "4", now);
  res.set_cookie(c);
}


int main(int argc, char** argv){

	micro::app application; 
	application.route("/hello", hello);
	application.run();

} 
