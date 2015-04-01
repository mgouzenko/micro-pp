#include <regex>
#include "url.hpp"

namespace micro {

    // A constructor for a catch-all URL that accepts any validly formed URL
    // This is going to be registered with a static file handler callback by default
    url::url(std::vector<std::string> methods, Callback callback)
    {
        callback_ = callback;
        methods_ = methods;
        internal_regex_ = std::regex("^/[A-Za-z0-9_\\-\\./]*$");
    }

    url::url(std::string specifier, std::vector<std::string> methods, Callback callback)
    {
        callback_ = callback;
        methods_ = methods;

        std::regex specifier_format ("^(/((<[A-Za-z0-9_\\-]+>)|(<int:[A-Za-z0-9_\\-]+>)|([A-Za-z0-9_\\-\\.]+)))*/?$");

        if(std::regex_match(specifier, specifier_format)) {
            std::string new_regex = "^" + specifier + "$";
            new_regex = std::regex_replace(new_regex, std::regex("<[A-Za-z0-9_\\-]+>"), "([A-Za-z0-9_\\-]+)");
            new_regex = std::regex_replace(new_regex, std::regex("<int:[A-Za-z0-9_\\-]+>"), "([0-9]+)");
            new_regex = std::regex_replace(new_regex, std::regex("[\\.]"), "\\.");

            // TODO: For debugging only
            std::cout << "Specifer " << specifier << " becomes regex " << new_regex << "\n";

            internal_regex_ = std::regex(new_regex);

            // Extract labels from the route specification
            std::regex label_extractor ("(?:<([A-Za-z0-9_\\-]+)>)|(?:<int:([A-Za-z0-9_\\-]+)>)");
            std::smatch m;
            std::vector<std::string> labels;
            while(std::regex_search (specifier, m, label_extractor)) {

                // This must be a standard label
                if (m[1] != "") {
                    labels_.push_back(m[1]);
                }
                // This must be an int label
                else {
                    labels_.push_back(m[2]);
                }

                specifier = m.suffix().str();
            }
        }
        else {
            std::cerr << "Fatal runtime error: \"" << specifier << "\" is not a valid URL route specifier.\nTerminating micro...\n";
            exit(1);
        }
    }

    // Should return true if it matches the request and populates the request with the relevant
    // returns false if the URL doesn't match the request
    bool url::match(http::server4::request& request)
    {

        bool allowable = false;

        // Check to see if the method of the request is a valid one for this URL
        for (auto method : methods_) {
            if(request.method == method) {
                allowable = true;
                break;
            }
        }

        // Check if the URL of the request matches this URL, and if it does,
        // populate the request with any relevant data
        std::smatch m;
        if(allowable && std::regex_match(request.uri, m, internal_regex_)) {

            // Extract the values and associate them with labels
            auto val_it = ++m.begin();
            auto label_it = labels_.begin();
            while (label_it != labels_.end()) {
                request.label_values.emplace(*label_it++, *val_it++);
            }

            return true;
        }
        return false;
    }

    // Should call the callback registered to this URL with the given request and the response to populate
    // TODO: potentially refactor to operator()?
    void url::callback(const http::server4::request& request, http::server4::reply& reply)
    {
        callback_(request, reply);
    }

}
