#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <boost/lexical_cast.hpp>

#include "request_handler.hpp"
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "types.hpp"
#include "server.hpp"
#include "url_route.hpp"

namespace micro {

void request_handler::operator()(server& serv)
{
  request& req = *(serv.request_);
  reply& rep = *(serv.reply_);

  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    serv();
    return; 
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep = reply::stock_reply(reply::bad_request);
    serv();
    return;
  }

  // TODO: Move to static file handler
  /*
  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_path[request_path.size() - 1] == '/')
  {
    request_path += "index.html";
}*/


  // Determine the file extension.
  // TODO: Extenstion should be set in the handle_response function
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
      extension = request_path.substr(last_dot_pos + 1);
  }

  try{

      bool matched = false;
      micro::response resp;
      auto callback_it = callback_routes_.begin();
      while(callback_it != callback_routes_.end() && !(matched = callback_it->match(req, resp)))
          ++callback_it;

      std::cout << "matched";
      if(!matched) {
          //TODO: Serve static file here I guess
          rep = reply::stock_reply(reply::not_found);
          serv();
          return; 
      }

        rep.handle_response(resp, extension);

   } catch(std::exception& e){
        rep = reply::stock_reply(reply::not_found);
        serv();
        return;
   }


  serv();
}

void request_handler::add_route(micro::url_route route)
{
    callback_routes_.push_back(route);
}

void request_handler::set_static_root(std::string root)
{
    static_root_ = root;
}

void request_handler::serve_static(const micro::request& req, micro::response& resp)
{
    return;
}

/*
* Takes a url that has encoded special characters and decodes to find
* the correc path
* param in: encoded url from request
* param out: decoded url
* return bool: true on success, false on fail
*/
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
