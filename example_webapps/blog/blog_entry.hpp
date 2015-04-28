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

    friend std::ostream& operator<<(std::ostream& os, const blog_entry& be);
};
