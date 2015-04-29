# Design Document

*Requirements*
- 5-15 pages
- Error handling resource management
- Feature set
- Interface design
- Optimizations
- Features for the convenience of users
- Ideas for release 1.2

## Our Project

In this project we built a micro web-framework for C++. Rather than building a web-framework that interfaced with a web server like Apache or Nginx we took the challenge to build our own web-server. To build the server we used Boost.Asio which is a C++ networking library providing developers with an asynchronous model for network and I/O operations.


## What is a Web Framework

At its core, a web-framework provides an interface to build web based applications. The interface provides application developers a clean and easy way to handle requests and send responses without dealing with the complexities of the strict HTTP protocol.

Web-Frameworks today work off the Model-View-Controller paradigm which modularizes code into three distinct sections. The Model represents the fundamental data structures that are persisted in a database. These objects are often modeled to be stored in SQL databases like MySQL or document databases like MongoDB. A View is a html page or template that gets rendered to the user. The are the web pages that a user of the web application will see. The controller is where the logic of the web application takes place. The controller maps callback functions to url routes which allow users to handle a request sent to the route and serve a response to the user who accessed the web application. Within the controller a developer typically performs CRUD (Create, Read, Update, and Delete) operations on objects defined by the model.


Some of the most popular services for web application frameworks include Django and Flask for Python, Rails for Ruby, and Express for NodeJs. Django and Rails are more complicated choices with more features while Flask and Express are more lightweight. For our web-framework, we used Flask and Express as inspiration and references in making design decisions.

## Web-Framework vs Web Server

A web server is a computer which stores, processes, and serves web pages to clients using the HTTP protocol. Although this sounds very similar to a web-framework, the key difference is that the framework sits on top of the web server providing a much more user friendly API for handling HTTP requests and serving HTTP responses.

Web-frameworks like Flask provide a simple web-server for development purposes. This server, however, is not intended for production use. The documentation reads:

“You can use the builtin server during development, but you should use a full deployment option for production applications. (Do not use the builtin development server in production.)""  

Consequently, web-frameworks like Flask interface with servers which implement WSGI (Web Server Gateway Interface). WSGI is an interface between web servers and web applications written in Python. This allows Flask applications to interface with much more robust servers like Apache or Nginx.

## Building a Web-App

The foundation of building a web-application begins with constructing an `micro::app` object, which essentially wraps an underlying web-server. We will discuss interfacing with the web server later.

### Basic Structure

The foundation of building a web-application begins with constructing an `micro::app` object, setting the route of a static file directory, and running the application. You can construct an app with a custom port and address but it defaults to port `8080` and address `0.0.0.0`. `micro::app` essentially wraps our web server which can be interfaced with through the `micro::app` API. In this example we have developed web application which serves static files.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
int main(int argc, char** argv) {
    micro::app application;
    application.set_static_root("./static");
    application.run();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Although this program runs an instance of the server it is limited to only serving static files from the route designated by the function `set_static_route("./static").

###Adding a Route

Imagine we want to send a "hello world" message to any user who accesses our web application with the url and route `www.example.com/hello`. To do this we call `application.add_route("./hello" hello_callback)` which will register the callback function `hello_callback` with the route `/hello`. When a client sends a request to the server with the route `www.example.com/hello`, `hello_callback` will be called, printing "hello world" in the browser of the user.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
micro::response hello_callback(const micro::request& req)
{
    res.render_string("hello world");
}

int main(int argc, char** argv) {
    micro::app application;
    application.set_static_root("./static");
    application.add_route("/hello", hello_callback);
    application.run();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

##Routing

Url routing requires two steps:

1. Registering a route
2. Defining a calback function for the route

###Registering a Route

In order to register a route, you must call `add_route("/path", callback)` on the micro::app object. The path string can have multiple forward slashes in order to allow for flexibility in how routes are defined. Here are examples of various valid routes that can be registered to different callback function:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
application.add_route("/api", api_callback);
application.add_route("/api/users", users_callback);
application.add_route("/api/users/admin", admin_callback)
application.add_route("/api/groups", groups_callback);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to add convenience to users we provide a system for generating dynamic routes. It would be ridiculous if you had to define a route for each user. Instead, you define a dynamic route like so:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
application.add_route("/api/user/<username>", api_callback);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

OR

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
application.add_route("/api/user/<int:id>", api_callback);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A user can define a route with an string or integer which is possible do to regex parsing on a url.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
"^(/((<[A-Za-z0-9_\\-]+>)|(<int:[A-Za-z0-9_\\-]+>)|([A-Za-z0-9_\\-\\.]+)))*/?$"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

With dynamic routes the application developer can define a route and extract the variable parameters in the request object passed into the callback function. Routes such as `www.example.com/api/user/1` and `www.example.com/api/user/2` can be processed differently within the same callback. This allows for more concise and modular code.

###The Callback Function

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
micro::response example_callback(const micro::request& req)
{
    // Handle request and response here
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Every route has an associated callback function with a single parameter: a `micro::request` object. The `micro::request` can be thought of as a wrapper for the strict HTTP request protocol. The request object is passed in by const reference given that a user does not modify a request. The application developer only extracts data stored in the request object and provides logic in the callback to handle the request appropriately. The response object is modifiable with a provided API that allows users to send back customized responses to the client. Notice that the return type of the callback function is micro::response. A response object must always be constructed and returned in the callback. We made this design choice because a server is always supposed to respond to a request. The design of the callback is largely borrowed from the design of the Javascript Express web framework as illustrated here:

####Express Callback Example

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.js}
app.get('/', function(req, res){
    res.send('hello world');
});
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Like Express, we originally passed the request and response as parameters into the callback. Although this may reduce two two lines of code required to construct and return a response object, it is also more error prone.

####Express Callback Potential Bug

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.js}
app.get('/', function(req, res){
    if("bug") {
        res.redirect("/bug")
    }

    // Do other stuff
});
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example, the callback will call any functions after the if statement even if `res.redirect("/bug")`. Although this bug could be avoided by having an else block or inserting an empty return statement, we believe it leads to bugs and can be avoided by the explicit return of a response.


###The Request Object

The request object is populated with data parsed from the HTTP request and transformed into a more user friendly C++ object.

####Example: Getting data From the Request Object

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
// Define callback
micro::response api_callback(const micro::request& req, micro::response& res)
{
    std::string user = req.get_url_param("username"); // user becomes "bjarne"

    // Logic on for handling the user
}

// Set callback function to dynamic route
application.add_route("/api/user/<username>", api_callback);

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example, if a user accesses the url `www.example.com/api/bjarne`, the application developer has access to the username "bjarne" which gets stored in the request object.

####Request API

`micro::request::get_cookie`

`micro::request::get_post_param`

`micro::request::get_query_param`

`micro::request::get_route_param`

`micro::request::get_hostname`

`micro::request::get_uri`

`micro::request::get_ip`

`micro::request::get_method`

###The Response Object

The response object is modified within the callback and will be sent as an appropriately formatted HTTP response after the callback function.

####Example: Setting Data in the Response Object

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
micro::response api_callback(const micro::request& req)
{
    std::string user = req.get_url_param("username");

    std::string message = "Hello " + user;

    res.render_string(message)
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the example above, if a client accesses the url `www.example.com/api/bjarne`, the client would receive a message "hello bjarne".

####Response API

`micro::response::append_message`

`micro::response::render_string`

`micro::response::render_filestream`

`micro::response::render_file`

`micro::response::add_cookie`

`micro::response::redirect`

`micro::response::render_status`

`micro::response::set_mime_type`

### Route Modules

For user convenience we added the ability to build route modules so that application developers can create more modular code with associated routes in separate files. For example if you have a route `/api/` with additional parameters such as `/api/users` or `api/groups` these routes can be put into a module and registered more conveniently together.  

## The Server
The server is built on a hybrid between an asynchronous and multi-threaded model. All client-server communication is done asynchronously. That is, new sockets are accepted asynchronously, data is read from these sockets asynchronously, and the response is sent back asynchronously. After a request is received in full, it is added to a thread safe work queue. Threads from a pool process each request, determine which callback to use, and then invoke that callback on the request. When the callback completes, the thread schedules the response for asynchronous write-back to the client. An ancillary thread periodically tracks the progress of the thread pool, canceling any threads that have run longer than allowed by the user-specified (or else, default) timeout.  

### Boost.Asio Library
The asynchronous portion of micro++ is built upon the Asio io_service. The io_service forms an asynchronous queue, constantly accepting new sockets, reading from open sockets, and sending responses back to clients. The asynchronous loop only rests when there is no work to be done.

### Optimizations
Asynchronous client-server communication ensures that our server never blocks when receiving and responding to requests. Furthermore, we made the design decision to execute callbacks on separate threads. This allows us to take true advantage of multiple processors and monitor the execution time of threads (so that they do not exceed the timeout). It must be noted that micro knows nothing about the implementation of callbacks, so it is up to the user to make sure that callback functions do not block. For network communications that may block - like querying a third-party API - users are encouraged to maintain a separate asynchronous io thread, perhaps using boost::asio::io_service.  

### Error Handling
We take care to make sure that exceptions within callback functions do not bring down the whole server. To achieve this, all callbacks are invoked in try-catch blocks. When debug mode is on, any exception that results from hitting a specific url endpoint is displayed in the browser. When debug mode is off, all exceptions result in the server returning a "500: internal server error" status. In either case, the exception is logged to the console. We hope to make error handling and debug mode more robust in the future.


## Installation
To make it easy to build, install, and use this library on virtually any Unix-like system, we used GNU autotools to manage our build chain. The Makefile.am file in the src/ directory defines all of the source files that are needed to build micro++, as well as all of the header files that need to be included on a user's system. With this file, and the boilerplate configure.ac file, a user simply needs to generate a configure script (along with other associated files) using `autoreconf -i`, and after running that configure script, they will have Makefiles that are generated correctly for their system.

These Makefiles let a user build the library with a simple `make` command, as well as conveniently install it and its headers in the /usr/local/ directory without any further configuration.

Using autotools makes it easy for a user to port the micro++ library to any Unix-like architecture that is capable of running boost. This means micro++ can run on anything from a Raspberry Pi to a supercomputer.

## Additions for 1.2

### JSON handling
One of the main advantages of writing a web-application in python or javascript is the easy conversion of objects into JSON. JSON has become the most popular convention for buiding public APIs. For python, different data types including strings, numbers, and arrays can all be stored in the same dictionary and easily converted into a JSON object. For javascript, any javascript object can be converted into JSON. Although custom methods could be created to parse C++ objects to JSON, this requires a lot of extra work compared to python and javascript. In version 1.2 we would like to build an interface that can assist with JSON serialization.  

### Templating
One missing feature from our framework is the ability to easily template HTML. Other languages have robust templating engines (such as Jinja for Python). However, we have not found a templating engine for C++ that is both well-maintained and simple. Since templating engines are of utmost importance to web development, we hope to address this issue in the future. One option is to write our own templating engine. Another option is to use an already existing - and perhaps orphaned - open-source templating engine, and incorporate it into our library with a simplified wrapper.  

### Interface with industry server like Nginx or Apache
As discussed above, most web-frameworks have interfaces to connect to production strength servers. In the future we would like to provide users with the abiliy to interface with a CGI protocol so that application developers can connect the web framework to servers like Apache or Nginx


### Allow interface for middle-wear
Adding an interface to connect middleware so that other developers can develop for this web framework. Express has an extensive collection of middleware libraries that provide useful functionality such as cookie signing and encryption and more robust multi-part HTTP content body parsers.

### Per-thread database connection
TODO

### Threadsafe metrics
TODO

### Secure session handling (HTTPS)
TODO

### Full HTTP 1.1 compliance
TODO
