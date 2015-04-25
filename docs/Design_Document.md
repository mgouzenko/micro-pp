*Requirements*
- 5-15 pages
- Error handling resource management
- Feature set
- Interface design
- Optimizations
- Features for the convenience of users
- Ideas for release 1.2

#Design Document

##Our Project

Our initial idea was to build a micro web framework for C++. Rather than building a web-framework that interfaced with a web server like Apache or Nginx we took the challenge to build our own web-server. To build the server we used Boost.Asio which is a C++ networking library providing developers with an asynchronous model for network and I/O operations.


##What is a Web Framework

At its core, a web-framework provides an interface to build web based applications. The interface provides user a clean and easy way to handle request objects and send response objects without dealing with the complexities of the strict HTTP protocol. 

Web-Frameworks today work off the Model-View-Controller paradigm which modularizes code into three distinct sections. The Model represents the fundamental data structures that are persisted in a database. These objects are often modeled to fit in a SQL database like MySQL or a document database like MongoDB. A View is a html page or template that gets rendered to the user. The views are the webpages viewable to the end user. The controller is where the logic of the web application takes place. The controller typically maps callback functions to dynamic url routes which allow users to handle a request object sent to the url route and serve a response to the computer which accessed the web application. Within the controller a user typically performs CRUD (Create, Read, Update, and Delete) operation on persisted model objects. 


Some of the most popular services for web application frameworks include Django and Flask for Python, Rails for Ruby, and Express for NodeJs. While Django and Rails are much more complicated choices, Flask and Express are much lightweight. Consequently, we used Flask and Express as inspiration in making critical design decisions.

##Web-Framework vs Web Server

A web server is a computer which stores, processes, and serves web pages to clients using HTTP protocol. Although this sounds very similar to a web-framework, the key difference is that the framework sits on top of the web server providing a much more user friendly API for handling HTTP requests and serving HTTP responses.

When developing with a web-framework with Flask, Flask provides a simple web-server for development purposes. This server, however, is not intended for production use. The documentation reads:

“You can use the builtin server during development, but you should use a full deployment option for production applications. (Do not use the builtin development server in production.)  

Consequently, web-frameworks like Flask interfaces with servers which implement WSGI interfaces. WSGI is an interface between web servers and web applications written in python Python. This allows Flask applications to interface with much more robust servers like Apache or Nginx. 
Building a Web-App

The foundation of building a web-application begins with constructing an `micro::app` object, which essentially wraps an underlying web-server. We will discussing interfacing with the web  server later. 

##Building a Web-App

###Basic Structure

The foundation of building a web-application begins with constructing an `micro::app` object, setting the route of a static file directory, and running the application. `micro::app` essentially wraps our web server which can be iterfaced through the `micro::app` API. In this this example we have developed the most basic HTTP server which essentially wraps an underlying web-server. We will discussing interfacing with the web server later. 

```cpp
int main(int argc, char** argv) {
    micro::app application;
    application.set_static_root("./static");
    application.run();
}
```

Although this program runs an instance of the server it is limited to only serving static files from the route designated by the function `set_static_route("./static"). 

###Adding a Route

Imagine we want to send a "hello world" message to any user who accesses our web application with the url and route `www.example.com/hello`. To do this we call `application.add_route("./hello" hello_callback)` which will register the callback function `hello_callback` so that when a client sends a request to the server with the route "hello", `hello_callback` will be called.

```cpp
void hello(const micro::request& req, micro::response& res)
{
    res.render_string("hello world");
}


int main(int argc, char** argv) {
    micro::app application;
    application.set_static_root("./static");
    application.add_route("/hello", hello);
    application.run();
}
```

Within the callback function `hello`, the application programmer can choose to send a message back to the user. In this example we wish to send the string "hello world". That is all. Now, when the program run and the client acesses `www.example.com/hello`, "hello world" will appear in the browser.  

##Routing

Url routing requires two steps:

1. Registering a route 
2. Defining a calback function for the route 

###Registering a Route

In order to register a route, you must call `add_route("/path", callback)` on the micro::app object. The path string can have multiple forward slashes in order to allow for flexibility in how routes are defined. Here are examples of various valid routes that can be registered to differnt callback function:

```cpp
application.add_route("/api", api_callback);
application.add_route("/api/users", users_callback);
application.add_route("/api/users/admin", admin_callback)
application.add_route("/api/groups", groups_callback);
```

In order to add convienece to users we provide a system for generating dynamic routes which gives users maxmium flexibility. It would be ridiculous if you had to define a route for each user. Insted, you define a dynamic route like so:

```cpp
application.add_route("/api/<username>", api_callback);

application.add_route("/api/user/<int:id>", api_callback);
```

A user can either define a route with an string or integer. With the feature the application developer can define a route and extract the variable parameters in the callback object which allows routes `www.example.com/api/user/1` and `www.example.com/api/user/2` to be processed differently within the same callback. This allows for more concice and modular code. This is made possible through a complicated regex expresion that parses routes and makes the variable url parameters available through the `request` object API.

###The Callback Function

```cpp
void example_callback(const micro::request& req, micro::response& res)
{
    // Handle request and response here
}

```

Every route has an associated callback function with two parameters: a `request` and `response` object. The `request` and `response` can be thought of as wrappers for the strict HTTP request and response protocol. The request object is passed in by const refrence given that a user does not modify a request. The application developer only extracts data stored in the request object and provides logic in the callback to handle the request appropriatly. The reponse object is modifiable with a provided API that allows user to send back customized responses to the client. Notice that the return type of the callback function is void. **WHY??** The design of the callback is largely borrowed from the design of the Javascript Express web framework as illustrated here:

####Express Callback Example

```js
app.get('/', function(req, res){
    res.send('hello world');
});
```

####The Request Object

The request object is populated with data parsed from the HTTP request and transformed into a more user freindly C++ object.

#####Example of Getting data From the Request Object

If a client accessed the url `www.example.com/api/bjarne` the string `user` would in this example would be "bjarne".

```cpp
void api_callback(const micro::request& req, micro::response& res)
{
    std::string user = req.get_url_param("username");

    // Logic on for handling the user
}

application.add_route("/api/<username>", api_callback);

```

Information on accessing data stored in the request object is provided in the API.


####The Response Object

The response object is modified within the callback and will be sent as an appropriatly formatted HTTP response after the callback function.

#####Example of Setting Data in the Response Object

If a client accessed the url `www.example.com/api/bjarne`, the client would recieve a message "hello bjarne".

```cpp
void api_callback(const micro::request& req, micro::response& res)
{
    std::string user = req.get_url_param("username");

    std::string message = "Hello " + user;

    res.render_string(message) 
}
```

Information on setting data in the response object is provided in the API.

###Using Route Modules

##The Server

###Boost.Asio Library
- How does the aysnc model work
- May want to compare to Node aync event loop which uses libuv written in C

###Optimizaions
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


