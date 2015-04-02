
#include "app.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "url.hpp"


void hello(const http::server4::request& req, http::server4::response& resp){
  resp.render_string("hello world!");
}

int main(int argc, char** argv){

	micro::url test1 ("/<this>/i.s/<int:id>/c0ol", {"GET"}, hello);
	micro::url test2 ("/hello", {"GET"}, hello);
	micro::url test3 ("/user/<int:id>/photo", {"GET"}, hello);
	micro::url test4 ("/", {"GET"}, hello);
	micro::url test5 ("/user/<name>/pro-fi_le", {"POST"}, hello);

	http::server4::request r;
    r.method = "GET";
	r.uri = "/yo/i.s/37/c0ol";
	assert(test1.match(r));
    assert(r.label_values["id"] == "37");
	r.uri = "/hello";
	assert(test2.match(r));
	r.uri = "/user/61347/photo";
	assert(test3.match(r));
	r.uri = "/";
	assert(test4.match(r));

    r.method = "POST";
	r.uri = "/user/adamchel/pro-fi_le";
	assert(test5.match(r));
    assert(r.label_values["name"] == "adamchel");

    r.method = "GET";
	r.uri = "/yo/i.s/3a7/c0ol";
	assert(!test1.match(r));
	r.uri = "/hell0";
	assert(!test2.match(r));
	r.uri = "/user/613a47/photo";
	assert(!test3.match(r));
	r.uri = "/sup";
	assert(!test4.match(r));
	r.uri = "/user/adamchel/profile";
	assert(!test5.match(r));

    std::cout << "ALL ASSERTIONS PASSED!!\n";

}
