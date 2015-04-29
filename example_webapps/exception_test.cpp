#include <micro/app.hpp>
#include <micro/request.hpp>
#include <micro/response.hpp>
#include <ctime>
#include <fstream>



micro::response excep(const micro::request&){
    micro::response res;
    throw std::exception();
    return res;
}

micro::response hello(const micro::request& req)
{
    micro::response res;
    res.render_string("hello world");
    return res;
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
