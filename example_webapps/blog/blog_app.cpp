#include <micro/app.hpp>
#include <micro/response.hpp>
#include <micro/request.hpp>
#include <sstream>
#include <fstream>
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

    render_fragment(page, "fragments/homepage_header.html");

    page << "<ul class=\"entries\">";
    for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry)
        page << *entry;
    page << "</ul>";

    render_fragment(page, "fragments/homepage_footer.html");

    resp.render_string(page.str());

    return resp;
}

micro::response new_entry(const micro::request &req) {
    micro::response resp;
    blog_entry new_entry{req.get_post_param("title"), req.get_post_param("body"), req.get_post_param("name")};
    entries.push_back(new_entry);
    resp.redirect("/");
}

int main(int argc, char **argv) {
    micro::app app;

    app.set_pool_size(8);

    app.add_route("/", homepage);
    app.add_route("/new", new_entry, {"POST"});

    app.run();
}
