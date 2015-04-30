# Design Document

## Our Project

In this project we built Micro++, a C++ web-framework with a built in web-server. To build the server we used Boost.Asio which is a C++ networking library providing developers with an asynchronous model for network and I/O operations.


## What is a Web Framework

At its core, a web-framework provides an interface to build web based applications. The interface provides application developers a clean and easy way to handle requests and send responses without dealing with the complexities of the strict HTTP protocol.

Web-Frameworks today work off the Model-View-Controller paradigm which modularizes code into three distinct sections. The Model represents the fundamental data structures that are persisted in a database. These objects are often modeled to be stored in SQL databases like MySQL or document databases like MongoDB. A View is a html page or template that gets rendered to the user. These are the web pages that a user of the web application will see in his or her browser. The controller is where the logic of the web application resides. The controller maps callback functions to URL routes which allow application developers to handle the request sent to the designated route and serve a custom response to the user who accessed the web application. Within the controller a developer typically performs CRUD (Create, Read, Update, and Delete) operations on objects defined by the model.

Some of the most popular services for web application frameworks include Django and Flask for Python, Rails for Ruby, and Express for NodeJs. Django and Rails are more complicated choices with more features while Flask and Express are more lightweight. Currently, the most popular web-framework for C++ is WT, which has a complicated "widget-centric" design unlike any popular framework on the market. Therefore, we decided to build our own micro framework in C++, using Flask and Express as inspiration given their lightweight yet robust design.

## Web-Framework vs Web Server

A HTTP web server is a software program which stores, processes, and serves web pages to clients using the HTTP protocol. Although this sounds very similar to a web-framework, the key difference is that the framework sits on top of a web server providing a much more user friendly API for handling HTTP requests and serving HTTP responses.

Web-frameworks like Flask provide simple built in web-servers for development purposes. This server, however, is not intended for production use. The documentation reads:

> “You can use the builtin server during development, but you should use a full deployment option for production applications. (Do not use the builtin development server in production.)""  

In order to scale, web-frameworks like Flask interface with production servers by implementing CGI (Common Gateway Interface) protocols. Python uses a slightly different protocol for python called WSGI (Web Server Gateway Interface). With WSGI, Flask applications can be run on robust, production ready servers like Apache or Nginx.

## Building a Web-App

### Basic Structure

The foundation of building a web-application begins with constructing an `micro::app` object, setting the route of a static file directory, and running the application. Underneath the hood, `micro::app` initiates an underlying web-server which can be interfaced through the `micro::app`. The default `micro::app` constructor sets the port to `8080` and address `0.0.0.0`. The constructor allows you to set the port and address to custom values. The `micro::app` has other customizable features documented in the API such as setting the thread count for the server, turning on debug mode, and setting the route for your static file directory. Below, we give an example of a simple web application which serves static files contained within the directory `./static`.

~~~{.cpp}
int main(int argc, char** argv) {
    micro::app application;
    application.set_static_root("./static");
    application.run();
}
~~~

Given that a file exists in the directory `./static` such as example_image.jpg, you can access this image through the browser at `localhost:8080/example_image.jpg`.

## Routing

The advantage of using a web-framework is that it provides an easy and clean method to attach callback function to URL endpoints. To make URL routing as user-friendly as possible, we require a two-step process:

1. Defining a callback function.
2. Register a route with the callback function.

### Step 1: The Callback Function

~~~{.cpp}
micro::response callback(const micro::request& req)
{
    // Handle request and response here
}
~~~

Every route has an associated callback function which passes a`micro::request` and returns a `micro::response`. `micro::request` is passed in by const reference given that a user does not modify a request. The application developer only extracts data stored in the request object, using this information to provide an appropriate `micro::response`. The `micro::response` object is modifiable and has an API providing customization for the `micro::response`.

A response object must always be constructed and returned in the callback. We made this design choice because the HTTP protocol require web servers to always respond to a HTTP request. The design of the callback is largely borrowed from the design of the Javascript Express web framework as illustrated here:

#### Express Callback Example

~~~{.js}
app.get('/', function(req, res){
    res.send('hello world');
});
~~~

Like Express, we originally passed the request and response as parameters into the callback. Although this may reduce two two lines of code, we noticed an increased potential for bugs.

#### Express Callback Potential Bug

~~~{.js}
app.get('/', function(req, res){
    if("bug") {
        res.redirect("/bug")
    }

    // Do other stuff
});
~~~

In the example above, all function calls after `res.redirect("/bug")` will be called. Although this bug could be avoided by having an else block or inserting an empty return statement, we believe it leads to bugs and can be avoided by the explicit return of a response.

### Anonymous Callbacks

Like Express, users can provide an anonymous callback using a lambda expression, but we recommend users to define their callback functions for clearer coding style. In the example below, accessing the route `/lambda` returns the message "hello lambda."

~~~{.cpp}

  application.add_route("/lambda", [](const micro::request& req) {
        micro::response res;
        res.render_string("hello lambda");
        return res;
    });

~~~


### The Request Object

The request object is populated with data parsed from the HTTP request and transformed into a more user friendly C++ object.

#### Example: Getting data From the Request Object

~~~{.cpp}
// Define callback
micro::response api_callback(const micro::request& req)
{
    micro::response res;
    std::string user = req.get_url_param("username"); 

    // Logic on for handling the user
}

// In the main method, set the callback function to dynamic route
application.add_route("/api/user/<username>", api_callback);

~~~

In the example above, if a user accesses the URL `www.example.com/api/bjarne`, access to the username "bjarne" is available through the `micro::request::get_url_param` method.

Other methods available for `micro::request` can be viewed in the API.


### The Response Object

The response object is modified within the callback and will be sent as an appropriately formatted HTTP response after the callback function has been executed.

#### Example: Setting Data in the Response Object

~~~{.cpp}
micro::response api_callback(const micro::request& req)
{
    micro::response res;
    std::string user = req.get_url_param("username");
    std::string message = "Hello " + user;
    res.render_string(message)
    return res;
}
~~~

In the example above, if a client accesses the URL `www.example.com/api/bjarne`, the client would receive a message "hello bjarne".

Other methods available for `micro::response` can be viewed in the API.


### Step 2: Registering a Route

After defining a callback function, the callback can be registered for a particular route. In order to register a route, you must call `micro::app::add_route` on the `micro::app` instance. Here are examples of various valid routes that can be registered to different callback functions:

~~~{.cpp}
application.add_route("/api", api_callback);
application.add_route("/api/users", users_callback);
application.add_route("/api/groups", groups_callback);
~~~

### Dynamic Routes

In order to add convenience to users we provide a system for generating dynamic routes. It would be absurd to define a route for each user. Instead, you define a dynamic route like so:

~~~{.cpp}
application.add_route("/api/user/<username>", callback);
~~~

OR

~~~{.cpp}
application.add_route("/api/user/<int:id>", callback);
~~~

A user can define a route with an string or integer which is possible through complex regex parsing on the URL.

~~~{.cpp}
"^(/((<[A-Za-z0-9_\\-]+>)|(<int:[A-Za-z0-9_\\-]+>)|([A-Za-z0-9_\\-\\.]+)))*/?$"
~~~

With dynamic routes, the application developer can define a route and extract the variable parameters from `micro::response` in the callback. Routes such as `www.example.com/api/user/1` and `www.example.com/api/user/2` can be processed differently within the same callback. This allows for more concise and modular code.

### Defining the HTTP Method

Within the callback, users can check the HTTP method and respond to different methods using `if` statements.

~~~{.cpp}

micro::response test_methods(const micro::request& req)
{
    micro::response res;
    if (req.get_method() == "GET") {
        // Respond to GET request
        return res;
    }
    if (req.get_method() == "POST") {
        // Respond toe POST request
        return res;
    }
}
~~~

We found that this leads to messy and unnecessarily long callback functions. To encourage better code design, we give users the ability to define the HTTP method when registering the route so that multiple callbacks can be pointed at the same URL.

~~~{.cpp}
micro::response post(const micro::request& req)
{
    micro::response res;
    res.render_string("POST");
    return res;
}

micro::response get(const micro::request& req)
{
    micro::response res;
    res.render_string("GET");
    return res;
}

int main(int argc, char** argv){
    micro::app application;
    application.add_route("/get_or_post", get, {"GET"});
    application.add_route("/get_or_post", post, {"POST"});
    application.run();
}
~~~

This example shows the encouraged usage of multiple callbacks for the same URL. Such practices lead more compact and cleaner callbacks. This design was borrowed from Flask, which also allows you to define the method for the route.

~~~~{.py}
@app.route('/login', methods=['POST'])
def login():
    if request.method == 'POST':
        do_the_login()

~~~~

### Route Modules

For user convenience we added the ability to build route modules so that application developers can create more modular code with associated routes in separate files. The `micro::module` allows programmers to register routes to callbacks, but bind them to the application at a later time. For instance, suppose you want to include an admin module, which has a login, dashboard, and edit_user page. All of these things can be logically grouped together in an "admin" module. Suppose you already have callback functions for these three pages. Then, you can do: 

~~~{.cpp}
micro::module mod = { {"/login", admin_login},
                      {"/dashboard", admin_dashboard},
                      {"/edit_user", admin_edit_user} 
                    };
~~~

Later, we can register this module with the application, under the prefix "/admin/", as follows: 

~~~{.cpp}
app.add_module(mod, "/admin");
~~~

Now, all of the urls in the module will be accessible from the "admin" entry point. For instance, we can access `http://www.example.com/admin/login`. Suppose that from the login page, you want to redirect to the dashboard. This is a redirect within the module, so doing the following will not work:  

~~~{.cpp}
micro::response admin_login(micro::request& req){
    micro::response resp; 
    //Do login stuff.. 

    if(authorized) resp.redirect(/dashboard); 
    return resp;
}
~~~

The example above will not work, because it redirects to `/dashboard` instead of `/admin/dashboard`. Instead, modify the code as follows: 

~~~{.cpp}
micro::response admin_login(micro::request& req){
    micro::response resp; 
    //Do login stuff.. 

    if(authorized) resp.redirect(/dashboard, true); 
    return resp;
}
~~~

The `true` flag added to the redirect function indicates that we intend to redirect within the module. Now, the function will redirect to `/admin/dashboard`. 

The entire module can be constructed in a separate header file, which can be imported by the progammer. Thus, the intricacies of the module functionality are abstracted away. All that the end user of a module needs to do is to bind that module to the application! 

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


### Allow interface for middleware
Add an interface to connect middleware so that other developers can build libraries that work with Micro++. Express has an extensive collection of middleware libraries that provide useful functionality such as cookie signing and encryption and more robust multi-part HTTP content body parsers.

### Per-thread database connection
TODO

### Threadsafe metrics
TODO

### Full HTTP 1.1 compliance
TODO
