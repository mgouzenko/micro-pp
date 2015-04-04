
#include "app.hpp"
#include "request.hpp"
#include "response.hpp"
#include "cookie.hpp"
#include <ctime>


void hello(const micro::request& req, micro::response& res)
{
  time_t now = time(0) + 100;
  res.render_string("hello world");
  micro::Cookie c = micro::Cookie("fourth", "4", now);
  res.set_cookie(c);
}

void serve_number(const micro::request& req, micro::response& res)
{
    res.render_string("User ID requested: " + req.label_values.at("id"));
}

int main(int argc, char** argv){

	micro::app application;
	application.set_pool_size(4);
    application.add_route("/hello", {"GET"}, hello);
    application.add_route("/user/<int:id>/profile", {"GET"}, serve_number);
	application.run();

}
