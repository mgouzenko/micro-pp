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
    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main(){

    micro::app application("8000");
    application.toggle_debug_mode();
    application.toggle_logging_mode();
    application.set_pool_size(8);
    application.add_route("/hello", hello);
    application.add_route("/excep", excep);  
    application.run(); 
}
