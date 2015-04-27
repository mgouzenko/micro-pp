#ifndef __MICRO_MODULE_HPP__
#define __MICRO_MODULE_HPP__

#include "types.hpp" 
#include <vector> 

namespace micro{ 
    
    /**
     * **module_route** allows users of micro++ to group URL strings, callback functions, and method lists together, so that 
     * they may be bound to the application at a later time. Users can declare a **module**, which is typdef'd as a vector 
     * of **module_route**s. Then, the user can fill the **module** and register it with an instance of **app**. All **module_route**s 
     * within that **module** will be converted to url endpoints. 
     */
    class module_route{
        friend class app;    
        private: 
            std::string route_; 
            callback callback_; 
            std::vector<std::string> methods_;

        public: 
            /**
             * The constructor for a module route. This constructor follows the same semantics as the function **app::add_route**.  
             * @param route_specifier: the URL to bind the function to. See tutorial for information on capturing url parameters. 
             * @param func: the callback function, to be called when a client hits the URL endpoint.
             * @param methods: the allowable methods for this specific url endpoint, and this specific callback function.
             */
            module_route(std::string route_specifier, callback func, std::vector<std::string> methods = {"GET", "POST", "PUT", "DELETE"}){
                route_ = route_specifier; 
                callback_ = func; 
                methods_ = methods; 
            } 
    };

    typedef std::vector<module_route> module; 
}

#endif
