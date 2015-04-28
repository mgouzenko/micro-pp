#include <micro/app.hpp>
#include <micro/response.hpp>
#include <micro/request.hpp>
#include <sstream>
#include "blog_entry.hpp"

std::vector<blog_entry> entries;

micro::response homepage(const micro::request &req) {
    micro::response resp;
    std::ostringstream page;
    page << "<html><body><h1>Welcome to the blog!!</h1>";

    page << "<a href=\"/new\">New Entry</a><br/>";

    for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry)
        page << *entry;

    page << "</body></html>";

    resp.render_string(page.str());

    return resp;
}

micro::response new_entry(const micro::request &req) {
    micro::response resp;

    if(req.get_method() == "POST") {
        blog_entry new_entry{req.get_post_param("title"), req.get_post_param("body"), req.get_post_param("name")};
        entries.push_back(new_entry);
        resp.redirect("/");
        return resp;
    }
    else {
        resp.render_file("new_entry.html");
        return resp;
    }
}

int main(int argc, char **argv) {
    micro::app app;

    app.set_pool_size(8);

    app.add_route("/", homepage);
    app.add_route("/new", new_entry);

    app.run();
}
