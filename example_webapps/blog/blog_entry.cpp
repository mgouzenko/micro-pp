#include "blog_entry.hpp"
#include <iomanip>
#include <regex>

blog_entry::blog_entry(std::string title, std::string contents, std::string author)
{
    this->title = title;
    this->contents = contents;
    this->author = author;
    this->time_posted = std::time(nullptr);
}

std::ostream& operator<<(std::ostream& os, const blog_entry& be)
{
    os << "<h2>" << be.title << "</h2>";
    os << "<i>Posted by " << be.author;
    os << " on " << std::put_time(std::localtime(&(be.time_posted)), "%a %b %d");
    os << " at " << std::put_time(std::localtime(&(be.time_posted)), "%r");

    auto displayed_contents = std::regex_replace(be.contents, std::regex("\\n"), "<br/>");

    os << "</i></br>" << displayed_contents << "</br>";

    return os;
}
