#ifndef HTTP_SERVER4_MIME_TYPES_HPP
#define HTTP_SERVER4_MIME_TYPES_HPP

#include <string>

namespace http {
namespace server4 {
namespace mime_types {

    /// Convert a file extension into a MIME type.
    std::string extension_to_type(const std::string& extension);

} // namespace mime_types
} // namespace server4
} // namespace http

#endif // HTTP_SERVER4_MIME_TYPES_HPP
