
#include <micro/app.hpp>
#include <micro/request.hpp>
#include <micro/response.hpp>
#include <micro/cookie.hpp>
#include <ctime>
#include <fstream>
#include <unordered_map>
#include <cassert>

/**
* Recieves GET, POST, PUT, or DELETE request from client and returns
* the string of the designated request
*/
micro::response test_methods(const micro::request& req)
{
    micro::response res;
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
    return res;
}

/**
* Server should send a cookie to client
*/
micro::response test_one_cookie(const micro::request& req)
{
    micro::response res;
    time_t t = time(0) + 100;
    micro::cookie c = micro::cookie("cookie_key", "cookie_value", t, "/hello");
    res.add_cookie(c);
    return res;
}

/**
* Server should send two cookies to client
*/
micro::response test_two_cookies(const micro::request& req)
{
    micro::response res;
    micro::cookie c1 = micro::cookie("cookie_key1", "cookie_value1");
    micro::cookie c2 = micro::cookie("cookie_key2", "cookie_value2");
    res.add_cookie(c1);
    res.add_cookie(c2);
    return res;
}

/**
* Should get the cookie that the web browswer sends back to server
*/
micro::response test_get_cookies(const micro::request& req)
{
    micro::response res;
    //Python script sends two cookies, "cookie_key1" and "cookie_key2"
    std::string cookie_1 = req.get_cookie("cookie_key1");
    std::string cookie_2 = req.get_cookie("cookie_key2");
    std::string bad_cookie = req.get_cookie("cjdfkjf");
    assert(cookie_1 == "cookie_value1");
    assert(cookie_2 == "cookie_value2");
    assert(bad_cookie == "");

    res.render_string("cookie tests");
    return res;
}

/*
* Server should send 301 redirect with url path to /other
*/
micro::response test_redirect(const micro::request& req)
{
    micro::response res;
    res.redirect("/other");
    return res;
}

/**
* Test route for redirection. Responds with message 'You have been redirected'
*/
micro::response other(const micro::request& req)
{
    micro::response res;
    res.render_string("You have been redirected");
    return res;
}

/**
* Sends back 503 http code to client with default message
*/
micro::response test_bad_url(const micro::request& req)
{
    micro::response res;
    res.render_status(503);
    return res;
}

/**
* If user sends back an invalid http code, server responds with 500 status
*/
micro::response test_bad_url2(const micro::request& req)
{
    micro::response res;
    res.render_status(838338);
    return res;
}

/**
* Sends back 503 http code to client with custom message 'Custom 503 response'
*/
micro::response test_bad_url_custom(const micro::request& req)
{
    micro::response res;
    res.render_status(503, "Custom 503 response");
    return res;
}

/**
* Sends back a 503 response code to client with custom message 'Custom 503 response'
* Alternative to test_bad_url_custom
*/
micro::response test_bad_url_custom2(const micro::request& req)
{
    micro::response res;
    res.render_status(503);
    res.render_string("Custom 503 response");
    return res;
}

/**
* Should send only one message to user if user accidently renders twice
*/
micro::response two_messages(const micro::request& req)
{
    micro::response res;
    res.render_string("Should not send");
    res.render_string("Should send");
    return res;
}

/**
* Should send the content of the post params
* Testing with post keys password and username
*/
micro::response post_params(const micro::request& req)
{
    micro::response res;
    std::string username = req.get_post_param("username");
    std::string password = req.get_post_param("password");
    std::string response_string = username + " " + password;
    res.render_string(response_string);
    return res;
}

/**
* Should send the content of the get params
* Testing with get querystring ?username=zach&password=pass
*/ 
micro::response get_params(const micro::request& req)
{
    micro::response res;
    std::string username = req.get_query_param("username");
    std::string password = req.get_query_param("password");
    std::string response_string = username + " " + password;
    res.render_string(response_string);
    return res;
}

/** 
* Should send back the username passed to url api/<user>
*/
micro::response dynamic_url(const micro::request& req)
{
    micro::response res;
    std::string username = req.get_route_param("username");
    res.render_string(username);
    return res;
}

/**
* Should send back username and user_id passed to url api/<user>/<int:id>
*/
micro::response dynamic_url2(const micro::request& req)
{
    micro::response res;
    std::string username = req.get_route_param("username");
    std::string id = req.get_route_param("id");
    res.render_string(username + " " + id);
    return res;
}

/** 
* Should return static html page test01.html
*/
micro::response get_static(const micro::request& req)
{
    micro::response res;
    res.redirect("/test01.html");
    return res;
}

/**
* Should return a json object and set mime type to application/json
*/
micro::response get_json(const micro::request)
{
    micro::response res;
    res.render_string("{\"message\": \"hello\"}", "application/json");
    return res;
}

micro::response post(const micro::request)
{
    micro::response res;
    res.render_string("POST");
    return res;
}

micro::response get(const micro::request)
{
    micro::response res;
    res.render_string("GET");
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
    application.add_route("/test_methods", test_methods);
    application.add_route("/test_one_cookie", test_one_cookie);
    application.add_route("/test_two_cookies", test_two_cookies);
    application.add_route("/test_get_cookies", test_get_cookies);
    application.add_route("/test_redirect", test_redirect);
    application.add_route("/other", other);
    application.add_route("/test_bad_url", test_bad_url);
    application.add_route("/test_bad_url_custom", test_bad_url_custom);
    application.add_route("/test_bad_url_custom2", test_bad_url_custom2);
    application.add_route("/test_bad_url2", test_bad_url2);
    application.add_route("/two_messages", two_messages);
    application.add_route("/post_params", post_params);
    application.add_route("/get_params", get_params);
    application.add_route("/api/<username>", dynamic_url);
    application.add_route("/api/<username>/<int:id>", dynamic_url2);
    application.add_route("/get_static", get_static);
    application.add_route("/get_json", get_json);
    application.add_route("/get_or_post", get, {"GET"});
    application.add_route("/get_or_post", post, {"POST"});
    application.run();
}
