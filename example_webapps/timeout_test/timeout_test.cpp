#include <micro/app.hpp>
#include <micro/request.hpp>
#include <micro/response.hpp>
#include <micro/cookie.hpp>
#include <ctime>
#include <fstream>

micro::response timeout(const micro::request& req)
{
    micro::response res;
    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    res.render_string("should not be reached"); 
    return res;
}

int main(){
    micro::app application;
    application.add_route("/timeout", timeout);
    application.run();
}
