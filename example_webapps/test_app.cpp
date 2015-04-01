
#include "app.hpp"
#include "request.hpp" 
#include "response.hpp"


void hello(const http::server4::request& req, http::server4::response& res){
  std::string my_key = "third";
  std::string my_val = "3";
  res.render_string("hello world"); 
  res.set_cookie(my_key, my_val);
  //res.set_cookie("second", "2");
}


int main(int argc, char** argv){

	micro::app application; 
	application.route("/hello", hello);
	application.run();

} 
