# Tutorial: Extending the bjarneblog with intermediate micro++ features

## Basic session handling with cookies

In a blogging app, we might only want to let users share posts if they are logged in. Let’s create a very simple authentication system that will let users post entries for an hour if they type log in with the correct hardcoded password.

First let’s create a fragment for a basic login prompt:

### fragments/login.html

~~~{.html}
<form action="login" method="POST" class="add-entry">
<dl>
<dt>Name:
<dd><input type="text" size="30" name="name">
<dt>Password:
<dd><input type="password" size="30" name="password">
<dd><input type="submit" value="Login">
</dl>
</form>
~~~

and let’s remove the name field in the new entry form:

### new_entry_form.html

~~~{.html}
<form action="new" method="POST" class="add-entry">
<dl>
<dt>Title:
<dd><input type="text" size="30" name="title">
<dt>Text:
<dd><textarea name="body" rows="5" cols="40"></textarea>
<dd><input type="submit" value="Share">
</dl>
</form>
~~~

What we want to do now is set up a callback for the “login” action, we want to conditionally display a login form or a new entry form on the homepage depending on whether or not you are logged in, and we want to make sure that new entries set the name of the author to the name of the author in the cookie from when the author logged in.

Let’s start by setting up a callback for “login” with the following changes to blog_app.cpp. Add the following line to main before `app.run()`:

~~~{.cpp}
app.add_route("/login", login, {"POST"});
~~~

Add the following includes to the top of the file:
~~~{.cpp}
#include <micro/cookie.hpp>
#include <ctime>
~~~

Add this constant somewhere in the top of the file:
~~~{.cpp}
static const std::string blog_passwd = "ILoveCplusplus";
~~~

And finally add the following callback somewhere else in the file:
~~~{.cpp}
micro::response login(const micro::request& req) {
    micro::response resp;

    if(req.get_post_param("password") == blog_passwd) {
        micro::cookie authenticated{"auth", "true", std::time(nullptr) + 3600};
        micro::cookie name_cookie{"name", req.get_post_param("name"), "/new"};
        resp.set_cookie(authenticated);
        resp.set_cookie(name_cookie);
    }
    else {
        micro::cookie authenticated{"auth", "failed", std::time(nullptr) + 60};
        resp.set_cookie(authenticated);
    }
    
    resp.redirect("/");
   
    return resp;
}
~~~

What this callback does is accept a username and password as POST parameters, and depending on their contents, either authenticates the client to use the blog or not, using cookies.

If the post parameter given as the password matches the password, we will give the client a cookie that asserts that they are authenticated for one hour (current time + 3600 seconds), and a cookie with their name from the POST param. This name cookie will only be sent to the server when accessing “/new”

If the password doesn’t match, the client gets an authentication failed cookie for 60 seconds, just so the homepage will tell the end user that authentication failed.

Obviously this is not secure whatsoever, but it’s a foundation on which you can build a more secure authentication system. An example of such a system is one where you have a table of hashes that are valid (each with expirations), and when a user types the correct password, you create a random hash, add it to the list, and give it to the client in a Cookie.

Now we just need to modify the homepage and new_page callbacks so that the user sees the prompt that is appropriate for them, and so that they can only post when authenticated.

~~~{.cpp}
micro::response homepage(const micro::request &req) {
    micro::response resp;
    std::ostringstream page;

    render_fragment(page, "fragments/header.html");
    
    if(req.get_cookie("auth") == "true") {
        render_fragment(page, "fragments/new_entry_form.html");
    }
    else if(req.get_cookie("auth") == "failed") {
        page << "<strong>Authentication failed!</strong><br/>";
        render_fragment(page, "fragments/login.html");
    }
    else {
        render_fragment(page, "fragments/login.html");
    }
    
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
    
    if(req.get_cookie("auth") == "true" 
        && !req.get_post_param("body").empty() 
        && !req.get_post_param("title").empty()) 
    {
        blog_entry new_entry{req.get_post_param("title"), req.get_post_param("body"), req.get_cookie("name")};
        entries.push_back(new_entry);
    }
    resp.redirect("/");
    return resp;
}

~~~

As you can see in `homepage()`, we’ve added an if-else chain in the homepage that selectively renders a login form, a login form and authentication error, or a new entry form.

In `new_entry()`, we’ve changed the author to `req.get_cookie(“name”)`, and we only create a new entry if the client is authenticated. 

With the theme of adding more security to the app, we’ve also put checks on the title and body to ensure that users can’t make posts with empty parameters.

Now we have a simple, nice-looking blogging app with basic authentication!

## Producing permalinks to blog entries (URL route params)
If a user sees a blog post that they want to remember for future reference, it would be nice if they could get a permanent link to it. We can easily accomplish this with URL route parameters.

Let’s create a new route and callback so the end user can access a blog entry by its id.

First create the following route specification in `main()`

~~~{.cpp}
app.add_route("/entry/<int:id>", get_entry);
~~~

This creates a route that will accept URLs in the form of /entry/1235 or /entry/897. The “id” value will be available in the request object in the callback by accessing req.get_route_param(“id”).

You can do it like this:

~~~{.cpp}
micro::response get_entry(const micro::request &req) {
    micro:: response resp;
    std::ostringstream page;
   
    int id = std::stoi(req.get_route_param("id"));
    
    render_fragment(page, "fragments/header.html");
    page << "<a href=\"/\"><< Homepage</a><br/>";
    
    if(id > entries.size() || id < 1) {
        page << "<br/><h2>404 - Entry not found </h2>";
        render_fragment(page, "fragments/footer.html");
        resp.render_status(404, page.str());
    }
    else {
        page << "<ul class=\"entries\">";
        page << entries[id - 1];
        page << "</ul>";
        render_fragment(page, "fragments/footer.html");
        resp.render_string(page.str());
    }
    
    return resp;
}
~~~

As you can see, here we render a blog entry as a list of one entry (to maintain the formatting), or render a 404 status with the custom page.str() message if the requested entry id is not found. Now if you have three entries in the blog, visiting `localhost:8080/entry/2` will give you the second entry on a nice formatted page. Now we just need to give users this permalink by changing the for loop in the `homepage()` callback to this:

~~~{.cpp}
page << "<ul class=\"entries\">";
int i = entries.size();
for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry) {
    page << *entry;
    page << "<a href=/entry/" << i-- << ">Permalink</a><br/>";
}
page << "</ul>";

~~~

Now, each entry on the homepage will come with an associated permalink that actually works! If you want to make this look prettier (or you just want more practice with micro++), try embedding the entry id in blog_entry object, and making the title of the rendered blog entry a link to the correct permalink.

## Getting all of the articles written by a certain user (GET params)

It would also be nice if you could easily get all of the blog entries written by a particular user. We’re going to accomplish this with query string GET parameters. By doing this, we don’t even need to add any callbacks. By the end of this section, the query `localhost:8080/?u=Bjarne` should only display blog entries written by the user Bjarne.

This is actually extremely simple. Go to the `homepage()` and make the following modifications to the main for loop

~~~{.cpp}
page << "<ul class=\"entries\">";
    int i = entries.size();
    auto user = req.get_query_param("u");
    for (auto entry = entries.rbegin(); entry != entries.rend(); ++entry) {
        if(user == "" || user == entry->get_author()) {
            page << *entry;
            page << "<a href=/entry/" << i << ">Permalink</a><br/>";
        }
        --i;
    }
    page << "</ul>";
~~~

and add the following method definition in `blog_entry.hpp`
~~~{.cpp}
std::string get_author() { return author; }
~~~

Here you can see we get the query_param “u”, and if it isn’t blank, we only display entries where the author matches the “u” param. Notice that we moved the i decrement out of the if statement so that proper permalinks are still dsplayed.

Now you can simply type “localhost:8080/?u=SomeUser” in your browser and you’ll get selective results.

As an exercise, try building a search bar on the homepage that will send a get request to the homepage with the u query so that users can easily search for posts from their favorite authors.