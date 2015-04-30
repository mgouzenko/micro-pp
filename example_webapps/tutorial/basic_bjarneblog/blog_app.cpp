#include <sstream>
#include <micro/app.hpp>
#include <micro/response.hpp>
#include <micro/request.hpp>

#include "blog_entry.hpp"

std::vector<blog_entry> entries;

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
    render_fragment(page, "fragments/new_entry_form.html");
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
    blog_entry new_entry{req.get_post_param("title"), req.get_post_param("body"), req.get_post_param("name")};
    entries.push_back(new_entry);
    resp.redirect("/");
    return resp;
}

int main(int argc, char **argv) {
    micro::app app("3000"); 
    app.add_route("/", homepage);
    app.add_route("/new", new_entry, {"POST"});
    app.run();
}
