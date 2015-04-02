#ifndef HTTP_SERVER4_HEADER_HPP
#define HTTP_SERVER4_HEADER_HPP

#include <string>

namespace micro {

    struct header {
      std::string name;
      std::string value;
    };

} // namespace micro

#endif // HTTP_SERVER4_HEADER_HPP
