//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <signal.h>
#include "server.hpp"
#include "file_handler.hpp"
#include "app.hpp"
#include <thread>
#include "request_handler.hpp"
#include <chrono>

namespace micro{

	app::app()
	: handler(".")
	{
	
	}

	void app::run()
	{
	  try
	  {

		std::vector<std::thread> workers;

		for(int i=0; i<1; i++){
			std::cout << "added worker\n";
			workers.push_back(std::thread(&app::handle_requests, this));
		}
		
		//for(int i=0; i<workers.size(); i++) workers[i].detach(); 

		boost::asio::io_service io_service;
		
		
		http::server4::server(io_service, "0.0.0.0", "8080", q)();
		// Wait for signals indicating time to shut down.
		boost::asio::signal_set signals(io_service);
		signals.add(SIGINT);
		signals.add(SIGTERM);
		#if defined(SIGQUIT)
		signals.add(SIGQUIT);
		#endif // defined(SIGQUIT)
		signals.async_wait(boost::bind(
			  &boost::asio::io_service::stop, &io_service));

		// Run the server.
		io_service.run();
	  }
	  catch (std::exception& e)
	  {
		std::cerr << "exception: " << e.what() << "\n";
	  }

	}

	void app::route(std::string url, Callback func){
		handler.route(url, func); 	
	}

	void app::handle_requests(){
		for(;;){
			if(!q.empty() ){
				auto serv = q.front();
				q.pop();
				handler(serv); 
			} else
				std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}
