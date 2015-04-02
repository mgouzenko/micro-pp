//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"
#include <functional>
#include <unordered_map>
#include "types.hpp"

namespace http {
namespace server4 {

request_handler::request_handler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

void request_handler::operator()(const request& req, reply& rep)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // TODO: Extract query string parameters here

  // TODO: Try matching the URLs here

  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_path[request_path.size() - 1] == '/')
  {
    request_path += "index.html";
  }



  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }

  try{

	  auto routeCallback = callback_urls.at(req.uri); 
	  routeCallback(req, response_);
    rep.handle_response(response_, extension);


	  // //Fill out the reply to be sent to the client.
	  // rep.status = reply::ok;

   //  // Make sure to set size of header vector
	  // rep.headers.resize(2);
	  // rep.headers[0].name = "Content-Length";
	  // rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
	  // rep.headers[1].name = "Content-Type";
	  // rep.headers[1].value = mime_types::extension_to_type(extension);
    // Need to get headers set by user
   } catch(std::exception& e){
   		rep = reply::stock_reply(reply::not_found);
   		return;
   }


}

void request_handler::route(std::string url, Callback func){
	callback_urls.emplace(url, func);
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

} // namespace server4
} // namespace http
