#ifndef __MICRO_MODULE_HPP__
#define __MICRO_MODULE_HPP__

#include "types.hpp" 
#include <vector> 

namespace micro{ 

    class module_route{
        friend class app;    
        private: 
            std::string route_; 
            callback callback_; 
            std::vector<std::string> methods_;

        public: 
            module_route(std::string route, callback call, std::vector<std::string> meth = {"GET", "POST", "PUT", "DELETE"}){
                route_ = route; 
                callback_ = call; 
                methods_ = meth; 
            } 
    };

    typedef std::vector<module_route> module; 
}

#endif
