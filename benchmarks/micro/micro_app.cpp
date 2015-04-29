#include <micro/app.hpp> 
#include <micro/request.hpp> 
#include <micro/response.hpp>

micro::response home(const micro::request& req){
    micro::response resp; 
    
    std::string fname = req.get_query_param("fname"); 
    std::string lname = req.get_query_param("lname"); 
    std::string color = req.get_cookie("color"); 

    resp.render_string("Hello " + fname + " " + lname + ", your favorite color is " + color + ".");
    return resp; 
}

int main(){
    micro::app application;
    application.set_static_root("../static"); 
    application.add_route("/", home); 
    application.run(); 
}
