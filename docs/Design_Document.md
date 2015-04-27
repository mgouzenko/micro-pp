*Requirements*
- 5-15 pages
- Error handling resource management
- Feature set
- Interface design
- Optimizations
- Features for the convenience of users
- Ideas for release 1.2

# Design Document

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

The foundation of building a web-application begins with constructing an `micro::app` object, setting the route of a static file directory, and running the application. You can construct an app with a custom port and address but it defaults to port `8080` and address `0.0.0.0`. `micro::app` essentially wraps our web server which can be iterfaced through the `micro::app` API. In this this example we have developed web application which essentially wraps an underlying web-server that serves static files.

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
void hello(const micro::request& req, micro::response& res)
{
    res.render_string("hello world");
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
int main(int argc, char** argv) {
    micro::app application;
    application.set_static_root("./static");
    application.add_route("/hello", hello);
    application.run();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

##Routing

Url routing requires two steps:

1. Registering a route
2. Defining a calback function for the route

###Registering a Route

In order to register a route, you must call `add_route("/path", callback)` on the micro::app object. The path string can have multiple forward slashes in order to allow for flexibility in how routes are defined. Here are examples of various valid routes that can be registered to differnt callback function:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
application.add_route("/api", api_callback);
application.add_route("/api/users", users_callback);
application.add_route("/api/users/admin", admin_callback)
application.add_route("/api/groups", groups_callback);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In order to add convienece to users we provide a system for generating dynamic routes. It would be ridiculous if you had to define a route for each user. Insted, you define a dynamic route like so:

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

With dynamic routes the application developer can define a route and extract the variable parameters in the request object passed into the callback function. Routes such as `www.example.com/api/user/1` and `www.example.com/api/user/2` can be processed differently within the same callback. This allows for more concice and modular code.

###The Callback Function

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
void example_callback(const micro::request& req, micro::response& res)
{
    // Handle request and response here
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Every route has an associated callback function with two parameters: a `request` and `response` object. The `request` and `response` can be thought of as wrappers for the strict HTTP request and response protocol. The request object is passed in by const refrence given that a user does not modify a request. The application developer only extracts data stored in the request object and provides logic in the callback to handle the request appropriatly. The reponse object is modifiable with a provided API that allows users to send back customized responses to the client. Notice that the return type of the callback function is void. **WHY??** The design of the callback is largely borrowed from the design of the Javascript Express web framework as illustrated here:

####Express Callback Example

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.js}
app.get('/', function(req, res){
    res.send('hello world');
});
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

###The Request Object

The request object is populated with data parsed from the HTTP request and transformed into a more user freindly C++ object.

####Example: Getting data From the Request Object

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
// Define callback
void api_callback(const micro::request& req, micro::response& res)
{
    std::string user = req.get_url_param("username"); // user becomes "bjarne"

    // Logic on for handling the user
}

// Set calback function to dynamic route
application.add_route("/api/user/<username>", api_callback);

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example, if a user accesses the url `www.example.com/api/bjarne`, the application developer has access to the username "bjarne" which gets stored in the request object.

Information on accessing data stored in the request object is provided in the API.

###The Response Object

The response object is modified within the callback and will be sent as an appropriately formatted HTTP response after the callback function.

####Example: Setting Data in the Response Object

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
void api_callback(const micro::request& req, micro::response& res)
{
    std::string user = req.get_url_param("username");

    std::string message = "Hello " + user;

    res.render_string(message)
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the example above, if a client accesses the url `www.example.com/api/bjarne`, the client would recieve a message "hello bjarne".

Information on setting data in the response object is provided in the API.

###Route Modules

For user convienence we added the ability to build route modules so that application devopers can create more modular code with associated routes in seperate files. For example if you have a route `/api/` with additional paramers such as `/api/users` or `api/groups` these routes can be put into a module and registered more conviently together.  

##The Server

###Boost.Asio Library
The server is built on a hybrid between an asynchronous and multi-threaded model. All client-server communication is done asynchronously. That is, new sockets are accepted asynchronously, data is read from these sockets asynchronously, and the response is sent back asynchronously. After a request is received in full, it is added to a thread safe work queue. Threads from a pool process each request, determine which callback to use, and then invoke that callback on the request. When the callback completes, the thread schedules the response for asynchronous write-back to the client. An ancillary thread periodically tracks the progress of the thread pool, cancelling any threads that have run longer than allowed by the user-specified (or else, default) timeout.  

###Optimizaions
Asynchronous client-server communication ensures that our server never blocks when receiving and responding to requests. Furthermore, we made the design decision to execute callbacks on separate threads because this leaves the task of delegati

- Mitchell talk about optimizations with server with threading and queue

###Error Handling and Resource Management
- Mitchell talk about fancy error handling

##Installation
- Adam discuss fancy installation
- Focus on how this is a feature of convience for users

##Addions for 1.2
- Json handling
- Templating
- Interface with industry server like Nginx or Apache
- Allow iterface for middlewear (eg. parsing complex post request params)
