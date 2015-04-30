#include <sstream>
#include <micro/app.hpp>
#include <micro/response.hpp>
#include <micro/request.hpp>
#include <micro/cookie.hpp>
#include <ctime>

#include "blog_entry.hpp"

std::vector<blog_entry> entries;
static const std::string blog_passwd = "Cplusplus";

void render_fragment(std::ostringstream& page, std::string fragment_path)
{
    static char c;
    static std::ifstream ifs;
    ifs.open(fragment_path);
    while(ifs.get(c))
        page << c;
    ifs.close();
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
    int i = entries.size();
    auto user = req.get_query_param("u");
    for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry) {
        if(user == "" || user == entry->get_author()) {
            page << *entry;
            page << "<a href=/entry/" << i << ">Permalink</a><br/>";
        }
        --i;
    }
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

micro::response get_entry(const micro::request &req) {
    micro:: response resp;
    std::ostringstream page;
    int id = std::stoi(req.get_route_param("id"));
    render_fragment(page, "fragments/header.html");
    page << "<a href=\"/\"><< Homepage</a><br/>";
    if(id > entries.size() || id < 1) {
        page << "<br/><h2>404 - Entry not found </h2>";
        render_fragment(page, "fragments/footer.html");
        resp.render_status(404, page.str());
    }
    else {
        page << "<ul class=\"entries\">";
        page << entries[id - 1];
        page << "</ul>";
        render_fragment(page, "fragments/footer.html");
        resp.render_string(page.str());
    }
    return resp;
}

int main(int argc, char **argv) {
    micro::app app("3000"); 
    app.add_route("/", homepage);
    app.add_route("/entry/<int:id>", get_entry);
    app.add_route("/new", new_entry, {"POST"});
    app.add_route("/login", login, {"POST"});
    app.run();
}
