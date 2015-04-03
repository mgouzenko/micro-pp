#include "app.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "url_route.hpp"

void hello(const micro::request& req, micro::response& resp){
  resp.render_string("hello world!");
}

int main(int argc, char** argv){

	micro::url_route test1 ("/<this>/i.s/<int:id>/c0ol", {"GET"}, hello);
	micro::url_route test2 ("/hello", {"GET"}, hello);
	micro::url_route test3 ("/user/<int:id>/photo", {"GET"}, hello);
	micro::url_route test4 ("/", {"GET"}, hello);
	micro::url_route test5 ("/user/<name>/pro-fi_le", {"POST"}, hello);

	micro::request r;
    micro::response resp;

    r.method = "GET";
	r.uri = "/yo/i.s/37/c0ol";
	assert(test1.match(r, resp));
    assert(r.label_values["id"] == "37");
	r.uri = "/hello";

	assert(test2.match(r, resp));
	r.uri = "/user/61347/photo";
	assert(test3.match(r, resp));



	r.uri = "/";
	assert(test4.match(r, resp));

    r.method = "POST";
	r.uri = "/user/adamchel/pro-fi_le";
	assert(test5.match(r, resp));
    assert(r.label_values["name"] == "adamchel");

    r.method = "GET";
	r.uri = "/yo/i.s/3a7/c0ol";
	assert(!test1.match(r, resp));
	r.uri = "/hell0";
	assert(!test2.match(r, resp));
	r.uri = "/user/613a47/photo";
	assert(!test3.match(r, resp));
	r.uri = "/sup";
	assert(!test4.match(r, resp));
	r.uri = "/user/adamchel/profile";
	assert(!test5.match(r, resp));

    std::cout << "ALL ASSERTIONS PASSED!!\n";

}
