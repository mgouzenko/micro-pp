
#include <micro/app.hpp>
#include <micro/request.hpp>
#include <micro/response.hpp>
#include <micro/module.hpp>
#include <micro/cookie.hpp>
#include <ctime>
#include <fstream>


void hello(const micro::request& req, micro::response& res)
{
    time_t t = time(0) + 100;
    res.render_string("hello world");
    micro::Cookie c = micro::Cookie("fifth", "5", t, "/hello");
    res.set_cookie(c);
}

void test_redirect(const micro::request& req, micro::response& res)
{
    res.redirect("/other", true);
}

void other(const micro::request& req, micro::response& res)
{
    res.render_string("you have been redirected");
}

void bad_url(const micro::request& req, micro::response& res)
{
    res.render_status(503);
}

void get_stuff(const micro::request& req, micro::response& res)
{
    auto param1 = req.get_get_param("param1");
    auto param2 = req.get_get_param("param2");
    auto param3 = req.get_get_param("param3");
    res.render_string("<html><body>param1 : " + param1 + "<br/>param2 : " + param2 + "<br/>param3 : " + param3);
}

void bad_url_custom(const micro::request& req, micro::response& res)
{
    res.render_status(501, "Sorry Bro, its Broken");
}

void nothing(const micro::request& req, micro::response& res)
{
    //User returns nothing
    //Browser will stay on same page
}

void serve_number(const micro::request& req, micro::response& res)
{
    res.render_string("User ID requested: " + req.label_values.at("id"));
}

int main(int argc, char** argv){
    if(argc != 2) {
        std::cout << "Usage: test_app <static_file_root>\n";
        exit(1);
    }

    micro::app application;
    application.set_pool_size(8);
    application.set_static_root(argv[1]);
    micro::module mod = { {"/hello", hello},
                          {"/test_redirect", test_redirect},
                          {"/other", other},
                          {"/bad_url", bad_url},
                          {"/nothing", nothing},
                          {"/bad_url_custom", bad_url_custom},
                          {"/user/<int:id>/profile", serve_number},
                          {"/get_stuff", get_stuff}
                        };
    application.add_module(mod, "/module");
    application.run();
}
