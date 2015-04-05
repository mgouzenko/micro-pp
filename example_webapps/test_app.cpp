
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
    res.render_status(501);
}

void serve_number(const micro::request& req, micro::response& res)
{
    res.render_string("User ID requested: " + req.label_values.at("id"));
}

int main(int argc, char** argv){
    micro::app application;
    application.set_pool_size(4);
    application.add_route("/hello", {"GET"}, hello);
    application.add_route("/test_redirect", {"GET"}, test_redirect);
    application.add_route("/other", {"GET"}, other);
    application.add_route("/bad_url", {"GET"}, bad_url);
    application.add_route("/user/<int:id>/profile", {"GET"}, serve_number);
    application.run();
}
