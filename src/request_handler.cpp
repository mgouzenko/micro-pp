#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>

#include "request_handler.hpp"
#include "server.hpp"
#include "url_route.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "response.hpp"

namespace micro {

    void request_handler::invoke_server(server& serv){
        if(logging_mode_){
            BOOST_LOG_TRIVIAL(info)
                << "[" << serv.request_->client_ip << "] "
                << "\""
                    << serv.request_->method
                    << " "
                    << serv.request_->uri
                    << " HTTP/"
                    << serv.request_->http_version_major
                    << "."
                    << serv.request_->http_version_minor
                << "\" "
                << serv.reply_->status;
        }
        serv();
    }

    void request_handler::operator()(server& serv)
    {

        request& req = *(serv.request_);
        reply& rep = *(serv.reply_);


        // Decode url to path.
        std::string request_path;
        if (!url_decode(req.uri, request_path)){
            rep = reply::stock_reply(reply::bad_request);
            invoke_server(serv);
            return;
        }

        // Request path must be absolute and not contain "..".
        if (request_path.empty() || request_path[0] != '/' || request_path.find("..") != std::string::npos) {
            rep = reply::stock_reply(reply::bad_request);
            invoke_server(serv);
            return;
        }

        response resp;
        try{

            request_path = request_parser::format_request(req, request_path);

            bool matched = false;

            // Attempt to find a static file matching this name
            if (request_path[request_path.size() - 1] == '/'){
              matched = resp.render_file(static_root_ + request_path.substr(1) + "index.html");
            }
            else {
              matched = resp.render_file(static_root_ + request_path.substr(1));
            }

            resp.set_status_code(200);

            // Attempt to match the URL to a callback
            auto callback_it = callback_routes_.begin();
            if(!matched) {
                while(callback_it != callback_routes_.end() && !(matched = callback_it->match(req)))
                    ++callback_it;
            }

            if(matched) {

            // Execute the callback that populates the response
            resp = callback_it->callback_(req);

            // Set the module entry point of the response. 
            resp.module_entry_point_ = &(callback_it->module_entry_point_); 

            }
            else {
                resp.set_status_code(404);
            }

            // At this point, the response will contain a file, the contents as set by a callback, or a 401 as set by render_file
            //TODO: May want create a response handler to be consitent with request handler


        } catch(std::exception& e){
            BOOST_LOG_TRIVIAL(warning) << e.what();

            if(debug_mode_){
                resp.render_string(e.what() );
                resp.set_status_code(500);
            }
            else{
                rep = reply::stock_reply(reply::internal_server_error);
                invoke_server(serv);
                return;
            }
        }

        rep.handle_response(resp);
        invoke_server(serv);
    }

    void request_handler::add_route(url_route route)
    {
        callback_routes_.push_back(route);
    }

    void request_handler::set_static_root(std::string root)
    {
        static_root_ = root;
    }


    bool request_handler::url_decode(const std::string& in, std::string& out)
    {
      out.clear();
      out.reserve(in.size());
      for (std::size_t i = 0; i < in.size(); ++i)
      {
        //Special character decoded from %Hex values
        if (in[i] == '%')
        {
          if (i + 3 <= in.size())
          {
            int value = 0;
            std::istringstream is(in.substr(i + 1, 2));
            if (is >> std::hex >> value)
            {
              out += static_cast<char>(value);
              i += 2;
            }
            else
            {
              return false;
            }
          }
          else
          {
            return false;
          }
        }
        else if (in[i] == '+')
        {
          out += ' ';
        }
        else
        {
          out += in[i];
        }
      }
      return true;
    }

} // namespace micro
