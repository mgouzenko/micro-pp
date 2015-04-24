*Requirements*
•5-15 pages
•Error handling resource management
•Feature set
•Interface design
•Optimizations
•Features for the convenience of users
•Ideas for release 1.2

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

```
int main(int argc, char** argv){
    micro::app application;
    application.set_static_root("./static");
    application.run();
}
```

Although this program runs an instance of the server it is limited to only serving static files from the route designated by the function `set_static_route("./static"). 

###Adding a Route

Imagine we want to send a "hello world" message to any user who accesses our web application with the url and route `www.example.com/hello`. To do this we call `application.add_route("./hello" hello_callback)` which will register the callback function `hello_callback` so that when a client sends a request to the server with the route "hello", `hello_callback` will be called.

```
void hello(const micro::request& req, micro::response& res)
{
    res.render_string("hello world");
}


int main(int argc, char** argv){
    micro::app application;
    application.set_static_root("./static");
    application.add_route("/hello", hello);
    application.run();
}
```

Within the callback function `hello`, the application programmer can choose to send a message back to the user. In this example we wish to send the string "hello world". That is all. Now when the program is run and the user 

##Routing

###Registering a Route
- The complex regex

###The Request Object
- Talk about API 
- Comparisons to other frameworks
- Convienence to Users

###The Response Object
- Talk about API 
- Comparisons to other frameworks
- Convienence to Users

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


