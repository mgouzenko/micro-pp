#include "blog_entry.hpp"
#include <iomanip>
#include <regex>

std::string sanitize_input(std::string input)
{
    // Sanitize the entry for exploits
    auto output = std::regex_replace(input, std::regex("&"), "&amp;");
    output = std::regex_replace(output, std::regex("<"), "&lt;");
    output = std::regex_replace(output, std::regex(">"), "&gt;");
    output = std::regex_replace(output, std::regex("\\n"), "<br/>");

    return output;
}

blog_entry::blog_entry(std::string title, std::string contents, std::string author)
{
    this->title = title;
    this->contents = contents;
    this->author = author;
    this->time_posted = std::time(nullptr);
}

std::ostream& operator<<(std::ostream& os, const blog_entry& be)
{
    os << "<li><h2>" << sanitize_input(be.title) << "</h2>";
    os << "<em>Posted by " << sanitize_input(be.author);
    os << " on " << std::put_time(std::localtime(&(be.time_posted)), "%a %b %d");
    os << " at " << std::put_time(std::localtime(&(be.time_posted)), "%r");

    os << "</em><br/>" << sanitize_input(be.contents) << "</br>";

    return os;
}
