#ifndef HTTP_SERVER4_HEADER_HPP
#define HTTP_SERVER4_HEADER_HPP

#include <string>

namespace http {
namespace server4 {

    struct header {
      std::string name;
      std::string value;
    };

} // namespace server4
} // namespace http

#endif // HTTP_SERVER4_HEADER_HPP
