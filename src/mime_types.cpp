#include "mime_types.hpp"

namespace micro {
namespace mime_types {

    struct mapping {
      const char* extension;
      const char* mime_type;
    } mappings[] =
    {
      { "gif", "image/gif" },
      { "htm", "text/html" },
      { "html", "text/html" },
      { "css", "text/css"},
      { "jpg", "image/jpeg" },
      { "png", "image/png" },
      { "pdf", "application/pdf"},
      { "js", "application/javascript"},
      { 0, 0 } // Marks end of list.
    };

    std::string extension_to_type(const std::string& extension)
    {
      for (mapping* m = mappings; m->extension; ++m) {
        if (m->extension == extension) {
          return m->mime_type;
        }
      }

      return "text/plain";
    }

} // namespace mime_types
} // namespace micro
