#include <micro/app.hpp>
#include <micro/response.hpp>
#include <micro/request.hpp>
#include <micro/cookie.hpp>
#include <ctime>
#include <sstream>
#include <fstream>
#include "blog_entry.hpp"

std::vector<blog_entry> entries;
static const std::string blog_passwd = "ILoveCplusplus";

void render_fragment(std::ostringstream& page, std::string fragment_path)
{
    static char c;
    static std::ifstream ifs;
    ifs.open(fragment_path);
    while(ifs.get(c))
        page << c;
    ifs.close();
}

micro::response homepage(const micro::request &req) {
    micro::response resp;
    std::ostringstream page;

    render_fragment(page, "fragments/header.html");
    
    if(req.get_cookie("auth") == "true") {
        render_fragment(page, "fragments/new_entry_form.html");
    }
    else if(req.get_cookie("auth") == "failed") {
        page << "<strong>Authentication failed!</strong><br/>";
        render_fragment(page, "fragments/login.html");
    }
    else {
        render_fragment(page, "fragments/login.html");
    }
    
    page << "<ul class=\"entries\">";
    for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry)
        page << *entry;
    page << "</ul>";

    render_fragment(page, "fragments/footer.html");

    resp.render_string(page.str());

    return resp;
}

micro::response new_entry(const micro::request &req) {
    micro::response resp;
    
    if(req.get_cookie("auth") == "true" 
        && !req.get_post_param("body").empty() 
        && !req.get_post_param("title").empty()) 
    {
        blog_entry new_entry{req.get_post_param("title"), req.get_post_param("body"), req.get_cookie("name")};
        entries.push_back(new_entry);
    }
    resp.redirect("/");
    return resp;
}

micro::response hello(const micro::request& req) {
    micro::response resp;
    resp.render_string("<h1>Hello World!</h1>");
    return resp;
}

micro::response login(const micro::request& req) {
    micro::response resp;
    
    if(req.get_post_param("password") == blog_passwd) {
        micro::cookie authenticated{"auth", "true", std::time(nullptr) + 3600};
        micro::cookie name_cookie{"name", req.get_post_param("name"), "/new"};
        resp.set_cookie(authenticated);
        resp.set_cookie(name_cookie);
    }
    else {
        micro::cookie authenticated{"auth", "failed", std::time(nullptr) + 60};
        resp.set_cookie(authenticated);
    }
    
    resp.redirect("/");
   
    return resp;
}

int main(int argc, char **argv) {
    micro::app app;

    app.set_pool_size(8);

    app.add_route("/", homepage);
    app.add_route("/new", new_entry, {"POST"});
    app.add_route("/login", login, {"POST"});

    app.run();
}
