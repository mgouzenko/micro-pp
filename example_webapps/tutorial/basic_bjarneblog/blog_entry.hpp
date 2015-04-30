#include <string>
#include <ctime>
class blog_entry {

    friend std::ostream& operator<<(std::ostream& os, const blog_entry& be);

private:
    std::string title;
    std::string contents;
    std::string author;
    std::time_t time_posted;
public:
    blog_entry(std::string title, std::string contents, std::string author);
};
