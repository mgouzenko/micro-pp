#ifndef __MICRO_HEADER_HPP__
#define __MICRO_HEADER_HPP__

#include <string>
#include <ostream>

namespace micro {

    struct header {
      std::string name;
      std::string value;
    };

    inline std::ostream& operator<<(std::ostream& os, const header& h)
    {
        os << "Name: " << h.name << ", Val: " << h.value;
        return os;
    }

} // namespace micro

#endif // MICRO_HEADER_HPP
