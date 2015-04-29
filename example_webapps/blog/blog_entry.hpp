#include <string>
#include <ctime>
#include <ostream>

class blog_entry {
private:
    std::string title;
    std::string contents;
    std::string author;
    std::time_t time_posted;

public:

    blog_entry(std::string title, std::string contents, std::string author);

    std::string get_author() { return author; }
    /**
     * Renders the blog entry as an HTML <li> item
     */
    friend std::ostream& operator<<(std::ostream& os, const blog_entry& be);
};
