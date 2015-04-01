#include <regex>
#include "types.hpp"

namespace micro{

    class url{

    public:
        url(std::vector<std::string> methods, Callback callback); // Static file URL
        url(std::string specifier, std::vector<std::string> allowable_methods, Callback callback);

        bool match(http::server4::request request);
        void callback(const http::server4::request& request, http::server4::reply& reply);

    private:
        std::regex internal_regex_;
        std::vector<std::string> methods_;
        Callback callback_;
    };

}
