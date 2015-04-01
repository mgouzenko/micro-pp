#include "url.hpp"

namespace micro {

    // A constructor for a catch-all URL that accepts any validly formed URL
    // This is going to be registered with a static file handler callback by default
    url::url(std::vector<std::string> methods, Callback callback) {
        callback_ = callback;
        methods_ = methods;
        internal_regex_ = std::regex("^/[A-Za-z0-9\./]*$");
    }

    url::url(std::string specifier, std::vector<std::string> methods, Callback callback) {
        callback_ = callback;
        methods_ = methods;

        std::regex specifier_format ("^(/((<[A-Za-z0-9]+>)|(<int:[A-Za-z0-9]+>)|([A-Za-z0-9\.]+)))*/?$");

        if(std::regex_match(specifier, specifier_format)) {

            //TODO: This is where we'd eventually somehow extract labels
            // The code below is not even close to working
            /*
            std::vector<std::string> labels;
            std::regex label_regex ("(?:<[A-Za-z0-9]+>)|(?:<int:[A-Za-z0-9]+>)");

            std::cmatch results;

            std::regex_search(specifier.c_str(), results, label_regex);
            for (auto r : results) {
                std::cout << "New label: " << r << "\n";
                labels.push_back(r);

            }*/

            std::string new_regex = "^" + specifier + "$";
            new_regex = std::regex_replace(new_regex, std::regex("<[A-Za-z0-9]+>"), "([A-Za-z0-9]+)");
            new_regex = std::regex_replace(new_regex, std::regex("<int:[A-Za-z0-9]+>"), "([0-9]+)");
            new_regex = std::regex_replace(new_regex, std::regex("[\.]"), "\\.");

            // TODO: For debugging only
            std::cout << "Specifer " << specifier << " becomes regex " << new_regex << "\n";

            internal_regex_ = std::regex(new_regex);
        }
        else {
            std::cerr << "Fatal runtime error: \"" << specifier << "\" is not a valid URL route specifier.\nTerminating micro...\n";
            exit(1);
        }
    }

    // Should return true if it matches the request and populates the request with the relevant
    // returns false if the URL doesn't match the request
    bool url::match(http::server4::request request) {
        // TODO: make sure that the method of the request is one of the allowed methods of this URL
        // TODO: check if matching, if it is, populate the request with stuff
        // like the extracted labels and their values in this particular request
        return std::regex_match(request.uri, internal_regex_);
    }

    // Should call the callback registered to this URL with the given request and the response to populate
    void url::callback(const http::server4::request& request, http::server4::reply& reply) {
        callback_(request, reply);
    }

}
