
#include "app.hpp"
#include "request.hpp"
#include "response.hpp"
#include "cookie.hpp"
#include <ctime>
#include <fstream>

/**
* Recieves GET, POST, PUT, or DELETE request from client and returns
* the string of the designated request
*/
void test_1(const micro::request& req, micro::response& res)
{
    //time_t t = time(0) + 100;
    if (req.get_method() == "GET") {
        res.render_string("GET");
    }
    if (req.get_method() == "POST") {
        res.render_string("POST");
    }
    if (req.get_method() == "PUT") {
        res.render_string("PUT");
    }
    if (req.get_method() == "DELETE") {
        res.render_string("DELETE");
    }
    //micro::Cookie c = micro::Cookie("fifth", "5", t, "/hello");
    //res.set_cookie(c);
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
    if(argc != 2) {
        std::cout << "Usage: test_app <static_file_root>\n";
        exit(1);
    }

    micro::app application;
    application.set_pool_size(8);
    application.set_static_root(argv[1]);
    application.add_route("/test_1", test_1);
    application.add_route("/test_redirect", test_redirect);
    application.add_route("/other", other);
    application.add_route("/bad_url", bad_url);
    application.add_route("/nothing", nothing);
    application.add_route("/bad_url_custom", bad_url_custom);
    application.add_route("/user/<int:id>/profile", serve_number);
    application.run();
}
