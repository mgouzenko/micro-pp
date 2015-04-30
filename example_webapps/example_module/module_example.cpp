
#include <micro/app.hpp>
#include <micro/request.hpp>
#include <micro/response.hpp>
#include <micro/module.hpp>
#include <micro/cookie.hpp>
#include <ctime>
#include <fstream>


micro::response hello(const micro::request& req)
{
    micro::response res;
    res.render_string("hello world");
    return res;
}

micro::response test_module_redirect(const micro::request& req)
{
    micro::response res;
    res.redirect("/hello", true);
    return res;
}

micro::response goodbye(const micro::request& req)
{
    micro::response res;
    res.render_string("bye!");
    return res;
}

int main(int argc, char** argv){
    micro::app application;
    micro::module mod = { {"/hello", hello},
                          {"/test_module_redirect", test_module_redirect}
                        };
    application.add_module(mod, "/module");
    application.add_route("/goodbye", goodbye);
    application.run();
}
