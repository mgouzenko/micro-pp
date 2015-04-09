
#include "app.hpp"
#include "request.hpp"
#include "response.hpp"
#include "cookie.hpp"
#include <ctime>


void hello(const micro::request& req, micro::response& res)
{
    time_t t = time(0) + 100;
    res.render_string("hello world");
    micro::Cookie c = micro::Cookie("fifth", "5", t, "/hello");
    res.set_cookie(c);
}

void test_redirect(const micro::request& req, micro::response& res)
{
    res.redirect("/other");
}

void other(const micro::request& req, micro::response& res)
{
    res.render_string("you have been redirected");
}

void bad_url(const micro::request& req, micro::response& res)
{
    res.render_status(503);
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
    micro::app application;
    application.set_pool_size(4);
    application.add_route("/hello", hello);
    application.add_route("/test_redirect", test_redirect);
    application.add_route("/other", other);
    application.add_route("/bad_url", bad_url);
    application.add_route("/nothing", nothing);
    application.add_route("/bad_url_custom", bad_url_custom);
    application.add_route("/user/<int:id>/profile", serve_number);
    application.run();
}
