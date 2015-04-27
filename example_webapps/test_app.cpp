
#include <micro/app.hpp>
#include <micro/request.hpp>
#include <micro/response.hpp>
#include <micro/cookie.hpp>
#include <ctime>
#include <fstream>


micro::response hello(const micro::request& req)
{
    micro::response res;
    time_t t = time(0) + 100;
    res.render_string("hello world");
    micro::cookie c = micro::cookie("fifth", "5", t, "/hello");
    res.set_cookie(c);
    return res;
}

micro::response test_redirect(const micro::request& req)
{
    micro::response res;
    res.redirect("/other");
    return res;
}

micro::response other(const micro::request& req)
{
    micro::response res;
    res.render_string("you have been redirected");
    return res;
}

micro::response bad_url(const micro::request& req)
{
    micro::response res;
    res.render_status(503);
    return res;
}

micro::response get_stuff(const micro::request& req)
{
    micro::response res;
    auto param1 = req.get_query_param("param1");
    auto param2 = req.get_query_param("param2");
    auto param3 = req.get_query_param("param3");
    res.render_string("<html><body>param1 : " + param1 + "<br/>param2 : " + param2 + "<br/>param3 : " + param3);
    return res;
}

micro::response bad_url_custom(const micro::request& req)
{
    micro::response res;
    res.render_status(501, "Sorry Bro, its Broken");
    return res;
}

micro::response nothing(const micro::request& req)
{
    micro::response res;
    //User returns nothing
    //Browser will stay on same page
    return res;
}

micro::response serve_number(const micro::request& req)
{
    micro::response res;
    res.render_string("User ID requested: " + req.get_query_param("id"));
    return res;
}

int main(int argc, char** argv){
    if(argc != 2) {
        std::cout << "Usage: test_app <static_file_root>\n";
        exit(1);
    }

    micro::app application;
    application.set_pool_size(8);
    application.set_static_root(argv[1]);
    application.add_route("/hello", hello);
    application.add_route("/test_redirect", test_redirect);
    application.add_route("/other", other);
    application.add_route("/bad_url", bad_url);
    application.add_route("/nothing", nothing);
    application.add_route("/bad_url_custom", bad_url_custom);
    application.add_route("/user/<int:id>/profile", serve_number);
    application.add_route("/get_stuff", get_stuff);
    application.run();
}
