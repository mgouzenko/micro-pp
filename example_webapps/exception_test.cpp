#include "app.hpp"
#include "request.hpp"
#include "response.hpp"
#include "cookie.hpp"
#include <ctime>
#include <fstream>



void excep(const micro::request& req, micro::response& res){
    throw std::exception(); 
    return; 
}

void hello(const micro::request& req, micro::response& res)
{
    time_t t = time(0) + 100;
    res.render_string("hello world");
    micro::Cookie c = micro::Cookie("fifth", "5", t, "/hello");
    res.set_cookie(c);
}

int main(){

    micro::app application;
    application.set_pool_size(8);
    application.set_static_root(".");
    application.add_route("/hello", hello);
    application.add_route("/excep", excep);  
    application.run(); 
}
