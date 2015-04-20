
#include <micro/app.hpp>
#include <micro/request.hpp>
#include <micro/response.hpp>
#include <micro/cookie.hpp>
#include <ctime>
#include <fstream>

/**
* Recieves GET, POST, PUT, or DELETE request from client and returns
* the string of the designated request
*/
void test_methods(const micro::request& req, micro::response& res)
{
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
}

/**
* Server should send a cookie to client
*/
void test_one_cookie(const micro::request& req, micro::response& res)
{
    time_t t = time(0) + 100;
    micro::Cookie c = micro::Cookie("cookie_key", "cookie_value", t, "/hello");
    res.set_cookie(c);
}

/**
* Server should send two cookies to client
*/
void test_two_cookies(const micro::request& req, micro::response& res)
{
    time_t t = time(0) + 100;
    micro::Cookie c1 = micro::Cookie("cookie_key1", "cookie_value1", t, "/hello");
    micro::Cookie c2 = micro::Cookie("cookie_key2", "cookie_value2", t, "/hello");
    res.set_cookie(c1);
    res.set_cookie(c2);
}

/*
* Server should send 301 redirect with url path to /other
*/
void test_redirect(const micro::request& req, micro::response& res)
{
    res.redirect("/other");
}

/**
* Test route for redirection. Responds with message 'You have been redirected'
*/
void other(const micro::request& req, micro::response& res)
{
    res.render_string("You have been redirected");
}

/**
* Sends back 503 http code to client with default message
*/
void test_bad_url(const micro::request& req, micro::response& res)
{
    res.render_status(503);
}

/**
* Sends back 503 http code to client with custom message 'Custom 503 response'
*/
void test_bad_url_custom(const micro::request& req, micro::response& res)
{
    res.render_status(503, "Custom 503 response");
}

/**
* Should send only one message to user if user accidently renders twice
*/
void two_messages(const micro::request& req, micro::response& res)
{
    res.render_string("Should not send");
    res.render_string("Should send");
}


int main(int argc, char** argv){
    if(argc != 2) {
        std::cout << "Usage: test_app <static_file_root>\n";
        exit(1);
    }

    micro::app application;
    application.set_pool_size(8);
    application.set_static_root(argv[1]);
    application.add_route("/test_methods", test_methods);
    application.add_route("/test_one_cookie", test_one_cookie);
    application.add_route("/test_two_cookies", test_two_cookies);
    application.add_route("/test_redirect", test_redirect);
    application.add_route("/other", other);
    application.add_route("/test_bad_url", test_bad_url);
    application.add_route("/test_bad_url_custom", test_bad_url_custom);
    application.add_route("/two_messages", two_messages);
    application.run();
}
