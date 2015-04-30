# Tutorial: Building the basic bjarneblog

In this tutorial, we will teach you how to build a basic blogging application, step-by-step, using micro++. This tutorial will assume that you have installed micro++. If you haven't already, follow the installation instructions in README.md or on the homepage of the Doxygen output.

## Getting Started

A web-application is actually very simple and micro++ will help you get your application running very quickly. At its core a web-framework provides developers with an easy way to handle requests from users and send responses to these users. The micro++ framework defines a clean interface where you register a callback function to a specified route. Within a callback, the developer can easily access the contents of the request through the `micro::request` API and construct a `micro::response` that will be sent back to the client. This idea, though simple, lets you build incredibly flexible and even complex web applications that will run blazingly fast.

To get started, create a new directory where you will build the app, open up a new file named `blog_app.cpp`, and write the following code:

**blog_app.cpp**

~~~{.cpp}
#include <micro/app.hpp>
#include <micro/response.hpp>
#include <micro/request.hpp>

micro::response hello(const micro::request& req) {
    micro::response resp;
    resp.render_string("<h1>Hello World!</h1>");
    return resp;
}

int main(int argc, char **argv) {
    micro::app app;
    app.add_route("/", hello);
    app.run();
}
~~~

And create the following Makefile:

### Makefile

~~~{Makefile}
CXXFLAGS = -std=c++11
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread

blog_app: 
~~~

If you are on Linux, replace `lboost_log-mt` with `lboost_log`.

Now, when you run the compiled blog_app with `./blog_app`, opening up the url localhost:8080/ in your browser will display “Hello World!” in an h1 header.

This might seem like a lot of code for something so simple, but let’s go through the code in `main()` line by line in so you can understand exactly what’s going on.

~~~{.cpp}
micro::app app;
~~~

The foundation of building a web-application begins with constructing a `micro::app` object. The `micro::app` object can be thought of as a wrapper for a more complicated HTTP server. By default, the server `port` is set to `8080` but is easily customizable. Let’s try setting the server to our favorite port, “3000.” 

~~~{.cpp}
micro::app app("3000"); 
~~~

Recompile `blog_app.cpp` and run `./blog_app.` Your app will now be available on localhost:3000.

Let’s look at the next line in main:
~~~{.cpp}
app.add_route("/", hello);
~~~

This next line registers the route "/" with the hello callback. This means that whenever a client requests localhost:8080/ (i.e. the homepage), the `hello()` callback will be executed and the response constructed in the callback will be served to the end user. Let’s take a closer look at the callback function `hello()`:

~~~{.cpp}
micro::response hello(const micro::request& req) {
    micro::response resp;
    resp.render_string("<h1>Hello World!</h1>");
    return resp;
}
~~~

The definition of `hello` conforms to a strict callback interface defined by `micro::callback`. The callback must accept a const reference `micro::request` and must return a micro::response.

Within `hello()`, we perform three easy steps to render the message “hello world” to the user.
1. Construct a response object.
2. Render the string that will be sent back to the user. Note that we placed hello in html brackets. `render_string()` sets the default mime type to “text/html”. If you wished to sent a JSON “hello world” message, you could alternatively write `resp.render_string(“{\“message\”: \“hello world\”}”, “application/json”)` 
3. Finally, return the response.

Under the hood, the web-framework is constructing a properly formatted HTTP response. HTTP responses can get complicated and ugly. Wrapping the HTTP response in the `micro::response` creates a much cleaner interface.
 
To finish off this micro example, let’s look at the last line in `main()`:

~~~{.cpp}
app.run();
~~~

This line starts the HTTP server that powers micro++. A lot of magic happens when this command is executed. This server is starts running and registers all routes and their respective callbacks. Building a blog that returns “hello world” is not very satisfying so lets begin make this into a something that looks much more like a blog. Safely kill the server with the SIGINT signal (ctrl-C). 

## Creating a blog entry abstraction

micro++ gives you flexibility on how how you can implement your webapps. However, since micro++ web apps are built with C++, we encourage you to work in a clean object-oriented manner so your apps don’t become a large mess. 

In the next few sections we will build this blog app with some practices that we use, but as the programmer you are free to expand on and encouraged to improve them in other apps you build with micro++. 

First step in making a blog, we must define the object-oriented abstraction of a blog entry.

Here is how we decided to model a blog in its simplest form. 

### blog_entry.hpp

~~~{.cpp}
#include <string>
#include <ctime>

class blog_entry {
private:
    std::string title;
    std::string contents;
    std::string author;
    std::time_t time_posted;

public:

    blog_entry(std::string title, std::string contents, std::string author);

};
~~~

### blog_entry.cpp

~~~{.cpp}

#include "blog_entry.hpp"
#include <iomanip>
#include <regex>

blog_entry::blog_entry(std::string title, std::string contents, std::string author)
{
    this->title = title;
    this->contents = contents;
    this->author = author;
    this->time_posted = std::time(nullptr);
}
~~~

Our blog entry has a title, contents, author, and time posted. In our class file we define the constructor which sets all the instance variables. Notice that we included the header `<iomanip>` and `<regex>` which will be used later in the tutorial.  We are going to keep this example simple, but once finished with the tutorial you are encouraged to add more features such as followers, votes, likes, and comments. 

## Building the homepage

Now that we have modeled a simple blog entry, lets design our homepage. The homepage will have a title with a button that allows you to add a new entry. Let’s add a new route with a callback called `homepage()` which will be seen by the user when he or she first accesses the blog. Let’s also remove the `hello()` callback

### blog_app.cpp

~~~{.cpp}
#include <micro/app.hpp>
#include <micro/response.hpp>
#include <micro/request.hpp>
#include <sstream>

micro::response homepage(const micro::request &req) {
    micro::response resp;
    std::ostringstream page;

    page << "<html><body><h1>Welcome</h1>";

    page << "</body></html>";

    resp.render_string(page.str());

    return resp;
}

int main(int argc, char **argv) {
    micro::app app;

    app.add_route("/", homepage);

    app.run();
}

~~~

This snippet of code should look very similar to what we saw with the hello world example. There are a couple changes we would like to discuss. Starting from the top, you will see the additional header `#include <sstream>`. This allows us to use `std::ostringstream` which is a buffer that lets us build a string with stream operators. As we did before, we render a string which will be displayed in the browser of the user. Finally, the main `main` method looks exactly like the hello world example except the callback is named `homepage`.

Now, if we compile our blog and access `localhost:8080` we will see the welcome page of our application. Next, lets add a button to redirect our user to a new page so that he or she can write a blog entry.

## Adding a button to make a post

In the homepage callback, let’s add the a button to post a new entry:

### blog_app.cpp

~~~{.cpp}

micro::response homepage(const micro::request &req) {
    micro::response resp;
    std::ostringstream page;

    page << "<html><body><h1>Welcome</h1>";

    page << "<a href=\"/new\">New Entry</a><br/>";

    page << "</body></html>";

    resp.render_string(page.str());

    return resp;
}

~~~

Above you can see the addition of the line `page << "<a href=\"/new\">New Entry</a><br/>";`. WIthin the string we have a line of html code which is one way to add a redirection button. In this case, clicking “New Entry” will redirect to a new route `localhost:8080/new`, but it will not compile the server and click the button, you will see a 404 not found error because the route does not exist. Lets define this new route!

## Defining a route to add a new post

### blog_app.cpp

~~~{.cpp}
micro::response new_entry(const micro::request &req) {
    micro::response resp;

    return resp;
    }
}

~~~

Don’t forget to register the new route with the app.

### blog_app.cpp

~~~{.cpp}
int main(int argc, char **argv) {
    micro::app app;
    app.add_route("/", homepage);
    app.add_route("/new", new_entry);

    app.run();
}
~~~

Our callback for `new_entry` is not very exciting. We create a response, do nothing to it, and return the response. If we click the New Entry button, the server will respond with a 204 no content header. Some browsers are intelligent and will not redirect to a page that responds with a 204, so don’t be worried if nothing happens when you click the “New Entry” link. Lets make our our new page return an html form that gives users the ability to enter a new blog.

### Build and render a form for new blog entries

### new_entry.html

~~~
<html>
    <body>
        <a href="/">Homepage</a><br/>
        <form action="new" method="POST">
            Name:<br/>
            <input type="text" name="name">
            <br/>
            Title:<br/>
            <input type="text" name="title">
            <br/>
            <textarea name="body" rows="10" cols="30"></textarea>
            <br/><br/>
            <input type="submit" value="Submit">
        </form>
    </body>
</html>
~~~

Above you will see a simple html form that allows users to make entries into the blog. A generic template for an html form can be found with a simple google search. You can see that we have defined certain input types such as name, title, and body, which we defined as the model attributes in our `blog_entry` object above. Note the line `<form action="new" method="POST">`. This key line defines what happens when you press the submit button. When you click submit, it will send a `POST` request to the route `new`. We will discuss the implications of this later, but first let’s make this form visible to users who access the route `localhost:8080/new`, by adding a new method in our callback, `response::render_file`.

### blog_app.cpp

~~~{.cpp}
micro::response new_entry(const micro::request &req) {
    micro::response resp;

    resp.render_file(“new_entry.html”);
	
    return resp;
    }
}

~~~

Now, when you compile the app and access `localhost::8080/new` you will see the the beautifully rendered html file we just wrote. When you fill out the form and press submit, you will notice that the form cleared. As we discussed earlier, what actually happens is that the form is submitting a post request to the route `localhost:8080/new` with the parameters `name`, `title`, and `body`. Lets discuss how to handle this post request and transform the values into useful input.

### blog_app.cpp

~~~{.cpp}

std::vector<blog_entry> entries;

micro::response new_entry(const micro::request &req) {
    micro::response resp;

    if(req.get_method() == "POST") {
        blog_entry new_entry{req.get_post_param("title"), req.get_post_param("body"), req.get_post_param("name")};
        entries.push_back(new_entry);
        resp.redirect("/");
    }
    else {
        resp.render_file("new_entry.html");
    }

    return resp; 
}

~~~
Lets step through the new new code line by line. 

At the top of the code, we added a new global vector object to store all of the blog entries that people post to the blog.

In the `new_entry()` callback, we can see an if statement that checks the type method of the HTTP request. The request object has a method `micro::response::get_method` that allows you to check the method of the HTTP request. Common methods include POST, GET, PUT, and DELETE. 

We can see here that if the callback detects a POST, then it populates a new `blog_entry` object by extracting the post parameters. We can fetch the post parameters with the keys defined in the html form. We then push the new entry into the global vector we defined earlier. After entering the new blog entry, we use the method `response::redirect` to redirect to back to the homepage. 

If the request given is not a POST request, we render the new entry html as we did earlier. 

## Displaying the blog entries

Now that you have the capability of adding new blog entries to your vector, now you’ll want to be able to display the entries on the homepage. First, you must define the << operator so that you can load the HTML of a blog entry into the string buffer.

Add the following line to `blog_entry.hpp`

### blog_entry.hpp

~~~{.cpp}

friend std::ostream& operator<<(std::ostream& os, const blog_entry& be);

~~~

And the following function to `blog_entry.cpp`

###blog_entry.cpp

~~~{.cpp}
std::ostream& operator<<(std::ostream& os, const blog_entry& be)
{
    os << "<h2>" << be.title << "</h2>";
    os << "<em>Posted by " << be.author;
    os << " on " << std::put_time(std::localtime(&(be.time_posted)), "%a %b %d");
    os << " at " << std::put_time(std::localtime(&(be.time_posted)), "%r");

    auto displayed_contents = std::regex_replace(be.contents, std::regex("\\n"), "<br/>");

    os << "</em></br>" << displayed_contents << "</br>";

    return os;
}
~~~

Above, we have defined the `<<` operator for the blog entry in the header file and implemented the method in the class. Lets go through each line of code.

In the first four lines we print the the title, author, date, and time. We use the combination of the methods `std::put_time` and `std::local_time` in order to print the time into a nicely formatted string.

Next we call `std::regex_replace` on the contents of the blog entry in order to convert all newlines as `<br/>` statements. This is necessary because html does not know how to interpret `\n` statements. Finally we append the contents of the blog entry to the stream. 

This HTML rendering of a blog entry is not perfect however. If an end-user includes tags like `<b>` and `<i>` in their name, title, or message body, it can mess up the formatting of the rest of the blog. To fix this, we must sanitize the input!

Add the following function to `blog_entry.cpp`:

~~~{.cpp}
std::string sanitize_input(std::string input)
{
    // Sanitize the entry for exploits
    auto output = std::regex_replace(input, std::regex("&"), "&amp;");
    output = std::regex_replace(output, std::regex("<"), "&lt;");
    output = std::regex_replace(output, std::regex(">"), "&gt;");
    output = std::regex_replace(output, std::regex("\\n"), "<br/>");

    return output;
}
~~~

And make the following changes to the << operator:
~~~{.cpp}
std::ostream& operator<<(std::ostream& os, const blog_entry& be)
{
    os << "<li><h2>" << sanitize_input(be.title) << "</h2>";
    os << "<em>Posted by " << sanitize_input(be.author);
    os << " on " << std::put_time(std::localtime(&(be.time_posted)), "%a %b %d");
    os << " at " << std::put_time(std::localtime(&(be.time_posted)), "%r");

    os << "</em><br/>" << sanitize_input(be.contents) << "</br>";

    return os;
}
~~~

Now that we have defined how to print the the blog contents, all we need to do is print the contents onto our homepage!

## Printing the blog contents on the homepage

In order to view the blog posts on the homepage, we need to print them to the screen by updating the `homepage` callback.

### blog_app.cpp

~~~{.cpp}

micro::response homepage(const micro::request &req) {
    micro::response resp;
    std::ostringstream page;
    page << "<html><body><h1>Welcome to the blog!!</h1>";

    page << "<a href=\"/new\">New Entry</a><br/>";

    for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry)
        page << *entry;

    page << "</body></html>";

    resp.render_string(page.str());

    return resp;
}

~~~

Above you can see the new line of code added to print the blog entries one by one.

~~~{.cpp}
   for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry)
        page << *entry;
~~~

This line will loop through the entries which have been added to the blog and use the new `<<` we just wrote to print them to the screen.

Compile your blog and give it a run. By now you should have minimal running blog! Let’s recap what we have done so far. We defined `blog_entry` object, how that blog entry should be printed into html, as well as a few callbacks that allow you to post entries and view posted entries.

## Using stylesheets, images, and fragmented templates to style the blog

As you can probably tell, the simple blog you have now is not pretty or user friendly at all. We’re going to fix that by adding CSS stylesheets and consolidating the blog to a single styled and formatted page. Micro++ doesn’t come packaged with a templating system, so we’ll instead accomplish this with what we’ll call fragmented templates. 

This means we’ll put fragments of HTML files in a subdirectory called fragments/. Create the following three HTML files now:

### fragments/header.html

~~~{.html}
<!doctype html>
<title>bjarneblog</title>
<link rel="stylesheet" type="text/css" href="/style.css">
<body><div class="page">
<h1>Welcome to the bjarneblog!!</h1>
~~~

### fragments/footer.html

~~~{.html}
</div></body>
~~~

### fragments/new_entry_form.html

~~~{.html}
<form action="new" method="POST" class="add-entry">
<dl>
<dt>Name:
<dd><input type="text" size="30" name="name">
<dt>Title:
<dd><input type="text" size="30" name="title">
<dt>Text:
<dd><textarea name="body" rows="5" cols="40"></textarea>
<dd><input type="submit" value="Share">
</dl>
</form>
~~~

Let’s also create a style.css file in a new static/ subdirectory

### static/style.css

~~~{.css}
// Adapted from the stylesheet for the flask microblog example (flaskr)
body            { font-family: sans-serif; background: #eee; }
a, h1, h2       { color: #377BA8; }
h1, h2          { font-family: 'Verdana', sans-serif; margin: 0; }
h1              { border-bottom: 2px solid #eee; }
h2              { font-size: 1.2em; }

.page           { margin: 2em auto; width: 35em; border: 5px solid #ccc;
                  padding: 0.8em; background: white; }
.entries        { list-style: none; margin: 0; padding: 0; }
.entries li     { margin: 0.8em 1.2em; }
.entries li h2  { margin-left: -1em; }
.add-entry      { font-size: 0.9em; border-bottom: 1px solid #ccc; }
.add-entry dl   { font-family: sans-serif; font-weight: bold; }
.metanav        { text-align: right; font-size: 0.8em; padding: 0.3em;
                  margin-bottom: 1em; background: #fafafa; }
.flash          { background: #CEE5F5; padding: 0.5em;
                  border: 1px solid #AACBE2; }
.error          { background: #F0D6D6; padding: 0.5em; }
~~~

When you start a micro++ app, the ./static directory serves as a root for static files by default, so localhost:8080/style.css would serve the above file. This is why we link to /style.css in the `static/header.html` file.

Now we essentially have the building blocks to create any page on the site.

In blog_app.cpp, add the following function to the top of the file after the #includes:

~~~{.cpp}
void render_fragment(std::ostringstream& page, std::string fragment_path)
{
    static char c;
    static std::ifstream ifs;
    ifs.open(fragment_path);
    while(ifs.get(c))
        page << c;
    ifs.close();
}
~~~

This function takes in an ostringstream&, and appends the contents of a file to it. This will let us easily append our HTML fragments to a page that we’re building.

Let’s rewrite our homepage and new_entry callbacks to look like this:

~~~{.cpp}
micro::response homepage(const micro::request &req) {
    micro::response resp;
    std::ostringstream page;

    render_fragment(page, "fragments/header.html");
    render_fragment(page, "fragments/new_entry_form.html");

    page << "<ul class=\"entries\">";
    for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry)
        page << *entry;
    page << "</ul>";

    render_fragment(page, "fragments/footer.html");

    resp.render_string(page.str());

    return resp;
}

micro::response new_entry(const micro::request &req) {
    micro::response resp;
    blog_entry new_entry{req.get_post_param("title"), req.get_post_param("body"), req.get_post_param("name")};
    entries.push_back(new_entry);
    resp.redirect("/");
    return resp;
}
~~~

Lastly, change the addition of the new_entry route in `main()` so it only accepts post requests:

~~~{.cpp}
app.add_route("/new", new_entry, {"POST"});
~~~

This seems like a lot, but we’ve actually made the blog a lot simpler. First let’s look at the new `homepage()` callback. Now we simply build a response by rendering the fragments for a header, for the new entry form, and for the footer, with the code we had to print the blog entries between new entry form and the footer.

The new `new_entry()` callback will now simply extract the post requests for a new entry and redirect to the homepage. We can safely do this because we know this callback will only be reached when the request method is POST.

Now if you build and run the blog, you’ll see that it looks much nicer, and all of the functionality for the user takes place on the homepage.
