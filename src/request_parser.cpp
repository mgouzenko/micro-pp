#include <algorithm>
#include <cctype>
#include <iostream>
#include <regex>
#include <boost/lexical_cast.hpp>

#include "request_parser.hpp"
#include "request_handler.hpp"
#include "request.hpp"

namespace micro {

    #include <boost/asio/yield.hpp> // Enable the pseudo-keywords reenter, yield and fork.

    std::string request_parser::content_length_name_ = "Content-Length";

    boost::tribool request_parser::consume(request& req, char c)
    {
        // TODO: Extract query string parameters here
        // TODO: Extract POST parameters
      reenter (this) {
        req.method.clear();
        req.uri.clear();
        req.http_version_major = 0;
        req.http_version_minor = 0;
        req.headers.clear();
        req.content.clear();
        content_length_ = 0;

        // Request method.
        while (is_char(c) && !is_ctl(c) && !is_tspecial(c) && c != ' ') {
          req.method.push_back(c);
          yield return boost::indeterminate;
        }
        if (req.method.empty())
          return false;

        // Space.
        if (c != ' ') return false;
        yield return boost::indeterminate;

        // URI.
        while (!is_ctl(c) && c != ' ') {
          req.uri.push_back(c);
          yield return boost::indeterminate;
        }
        if (req.uri.empty()) return false;

        // Space.
        if (c != ' ') return false;
        yield return boost::indeterminate;

        // HTTP protocol identifier.
        if (c != 'H') return false;
        yield return boost::indeterminate;
        if (c != 'T') return false;
        yield return boost::indeterminate;
        if (c != 'T') return false;
        yield return boost::indeterminate;
        if (c != 'P') return false;
        yield return boost::indeterminate;

        // Slash.
        if (c != '/') return false;
        yield return boost::indeterminate;

        // Major version number.
        if (!is_digit(c)) return false;
        while (is_digit(c)) {
          req.http_version_major = req.http_version_major * 10 + c - '0';
          yield return boost::indeterminate;
        }

        // Dot.
        if (c != '.') return false;
        yield return boost::indeterminate;

        // Minor version number.
        if (!is_digit(c)) return false;
        while (is_digit(c)) {
          req.http_version_minor = req.http_version_minor * 10 + c - '0';
          yield return boost::indeterminate;
        }

        // CRLF.
        if (c != '\r') return false;
        yield return boost::indeterminate;
        if (c != '\n') return false;
        yield return boost::indeterminate;

        // Headers.
        while ((is_char(c) && !is_ctl(c) && !is_tspecial(c) && c != '\r')
            || (c == ' ' || c == '\t'))
        {
          if (c == ' ' || c == '\t') {
            // Leading whitespace. Must be continuation of previous header's value.
            if (req.headers.empty()) return false;
            while (c == ' ' || c == '\t')
              yield return boost::indeterminate;
          }
          else {
            // Start the next header.
            req.headers.push_back(header());

            // Header name.
            while (is_char(c) && !is_ctl(c) && !is_tspecial(c) && c != ':') {
              req.headers.back().name.push_back(c);
              yield return boost::indeterminate;
            }

            // Colon and space separates the header name from the header value.
            if (c != ':') return false;
            yield return boost::indeterminate;
            if (c != ' ') return false;
            yield return boost::indeterminate;
          }

          // Header value.
          while (is_char(c) && !is_ctl(c) && c != '\r') {
            req.headers.back().value.push_back(c);
            yield return boost::indeterminate;
          }

          // CRLF.
          if (c != '\r') return false;
          yield return boost::indeterminate;
          if (c != '\n') return false;
          yield return boost::indeterminate;
        }

        // CRLF.
        if (c != '\r') return false;
        yield return boost::indeterminate;
        if (c != '\n') return false;

        // Check for optional Content-Length header.
        for (std::size_t i = 0; i < req.headers.size(); ++i) {
          if (headers_equal(req.headers[i].name, content_length_name_)) {
            try {
              content_length_ =
                boost::lexical_cast<std::size_t>(req.headers[i].value);
            }
            catch (boost::bad_lexical_cast&) {
              return false;
            }
          }
        }

        // Content.
        while (req.content.size() < content_length_) {
          yield return boost::indeterminate;
          req.content.push_back(c);
        }
      }

      return true;
    }

    #include <boost/asio/unyield.hpp> // Disable the pseudo-keywords reenter, yield and fork.

    bool request_parser::is_char(int c)
    {
      return c >= 0 && c <= 127;
    }

    bool request_parser::is_ctl(int c)
    {
      return (c >= 0 && c <= 31) || (c == 127);
    }

    bool request_parser::is_tspecial(int c)
    {
      switch (c) {
      case '(': case ')': case '<': case '>': case '@':
      case ',': case ';': case ':': case '\\': case '"':
      case '/': case '[': case ']': case '?': case '=':
      case '{': case '}': case ' ': case '\t':
        return true;
      default:
        return false;
      }
    }

    bool request_parser::is_digit(int c)
    {
      return c >= '0' && c <= '9';
    }

    bool request_parser::tolower_compare(char a, char b)
    {
      return std::tolower(a) == std::tolower(b);
    }

    bool request_parser::headers_equal(const std::string& a, const std::string& b)
    {
      if (a.length() != b.length())
        return false;

      return std::equal(a.begin(), a.end(), b.begin(),
          &request_parser::tolower_compare);
    }

    std::string request_parser::format_request(request& req, std::string req_path)
    {
      bool has_post_params = false;
      for (auto h : req.headers) {

        //Fill the map of cookies
        if(h.name == "Cookie") {
            int equal_idx = h.value.find("=");
            std::string key = h.value.substr(0, equal_idx);
            std::string val = h.value.substr(equal_idx+1);
            req.cookies_[key] = val;
        }

        //Fill the hostname
        if (h.name == "Host") {
            req.hostname_ = h.value;
        }

        //TODO 1.2: Should be able to parse more compliecated multipart/formdata
        if (h.name == "Content-Type" && h.value == "application/x-www-form-urlencoded"){
            has_post_params = true;
        }
      }

      // Extract GET params

      int query_loc = req_path.rfind('?');

      if(query_loc != std::string::npos) {
          std::string query = req_path.substr(query_loc + 1);
          req_path = req_path.substr(0, query_loc);

          auto char_it = query.begin();
          while(char_it != query.end()) {
              std::string key("");
              std::string value("");

              while (char_it != query.end() && *char_it != '=') {
                  key += *char_it++;
              }

              if (char_it == query.end())
                  break;
              ++char_it;
              while (char_it != query.end() && *char_it != '&') {
                  value += *char_it++;
              }

              req.get_params_.emplace(key, value);
              if(char_it != query.end())
                  ++char_it;
          }
      }

      // Extract POST params
      if (has_post_params) {
          //Decode any special symbols
          std::string decoded_content;
          request_handler::url_decode(req.content, decoded_content);

          std::regex pattern("([\\w+%]+)=([^&]*)");
          auto words_begin = std::sregex_iterator(decoded_content.begin(), decoded_content.end(), pattern);
          auto words_end = std::sregex_iterator();

          for (std::sregex_iterator i = words_begin; i != words_end; i++)
          {
              std::string key = (*i)[1].str();
              std::string value = (*i)[2].str();
              req.post_params_[key] = value;
          }
      }

      req.uri = req_path;
      return req_path;
    }

} // namespace micro
