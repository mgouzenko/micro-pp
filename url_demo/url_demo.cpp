#include "app.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "url_route.hpp"

void hello(const micro::request& req, micro::response& resp){
  resp.render_string("hello world!");
}

void new_regex_tests()
{
    std::string s;
    std::string new_s;
    std::string query;



    std::cin >> s;
    while(s != "quit") {
        std::unordered_map<std::string, std::string> get_params_;
        new_s = s;
        query = "";
        int query_loc = s.rfind('?');

        if(query_loc != std::string::npos) {
            new_s = s.substr(0, query_loc);
            query = s.substr(query_loc + 1);
        }

        std::cout << "URI: " << new_s << "\n";
        std::cout << "Query: " << query << "\n";
        std::cout << "\n";

        auto char_it = query.begin();
        while(char_it != query.end()) {
            std::string key("");
            std::string value("");

            while (char_it != query.end() && *char_it != '=') {
                key += *char_it++;
            }
            if (char_it == query.end())
                break;
            ++char_it;
            while (char_it != query.end() && *char_it != '&') {
                value += *char_it++;
            }
            get_params_.emplace(key, value);
            if(char_it != query.end())
                ++char_it;
        }

        for (auto x : get_params_) {
            std::cout << x.first << " : " << x.second << "\n";
        }

        std::cin >> s;
    }
}


int main(int argc, char** argv){

    new_regex_tests();

    return 0;

	micro::url_route test1 ("/<this>/i.s/<int:id>/c0ol", hello);
	micro::url_route test2 ("/hello", hello);
	micro::url_route test3 ("/user/<int:id>/photo", hello);
	micro::url_route test4 ("/", hello);
	micro::url_route test5 ("/user/<name>/pro-fi_le", hello, {"POST"});

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
