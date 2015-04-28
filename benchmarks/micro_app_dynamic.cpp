#include <micro/app.hpp> 
#include <micro/request.hpp> 
#include <micro/response.hpp>
#include <micro/cookie.hpp>

using namespace std; 
micro::response home(const micro::request& req){
    micro::response resp; 
    
    string fname = request.get_cookie("fname"); 
    string lname = request.get_cookie("lname"); 
    string color = request.get_post_param("color"); 

    rep.render_string("Hello " + fname + " " + lname ", your favorite color is " + color + ".");
    return resp; 
}

int main(){
    micro::app application;
    application.add_route("/", home); 
    application.run(); 
}
