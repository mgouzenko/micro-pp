#ifndef MICRO_MIME_TYPES_HPP
#define MICRO_MIME_TYPES_HPP

#include <string>

namespace micro {
namespace mime_types {

    /**
     * Convert a file extension into a MIME type.
     *
     * @param extension: the file extension, such as "jpg" or "gif"
     */
    std::string extension_to_type(const std::string& extension);

} // namespace mime_types
} // namespace micro

#endif // MICRO_MIME_TYPES_HPP
