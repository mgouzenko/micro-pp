# Benchmarks: Speed Tests, Clarity, and Ease of Use. 

##Introduction 
In this section, we will compare **micro** to other web frameworks. Namely, we benchmark **micro** against **Flask**, a python framework, and **wt**, a widget-oriented C++ framework. In addition to speed tests, we examine source code for a "hello world" web application for all three frameworks. In our analyses, we comment on ease of use as well as speed. 

We have chosen to benchmark against **Flask** because this framework is a large inspiration for the type of functionality we hope to provide with **micro**. We have also chosen to benchmark against **wt** because it is the leading web framework for C++.  

##Speed Tests
In this section, we stress-tested all three frameworks using their built-in servers. For all tests, we used **Apache Benchmark** to send requests at a varying level of concurrency. In the first scenario, we had the frameworks render a dynamic response based on the content of the request. In the second scenario, we had each framework render a static image. In all tests, the webapps were run from within a two-core virtual machine and accessed via host-only networking - so that the results would not be influence by noise over the local network. The virtual machine has the following specs: 

<img src="../specs.png" alt="specs" style="width: 70vw"/>


###Dynamic Test
In the dynamic tests, we sent a single request containing both url and post parameters. The exact request sent was: 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
POST /?fname=Mitchell&lname=Gouzenko HTTP/1.1
Host: vbox:8080
Cache-Control: no-cache
Content-Type: application/x-www-form-urlencoded

color=blue
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For each web framwork, we created a simple webapp that would respond with the single message: "Hello <fname> <lname>, your favorite color is <color>." (The appropriate parameters were of course inserted into this message.) We sent 10,000 requests to each application - first at a concurrency level of 100 and then at a concurrency level of 10. The results of the tests are below.   

<img src="../dynamic.png" alt="Dynamic" style="width: 70vw"/>

In terms of total time taken to respond to the requests, **micro** was fastest for both levels of concurrency. In second place was **Flask** and **wt** took last place. However, we must be careful in our analysis. A more accurate picture can be observed by looking at bytes transferred per second for each framework. In this case, **wt** takes the lead, with **micro** in second place and **Flask** in third. 

This leads us to take note of the fact that **wt** sent a total of about 48 million bytes, approximately 48 times the bytes that **micro** sent, and 24 times the bytes that **Flask** sent. That is, **wt** clearly has the largest footprint of the three frameworks. **wt** has a heavy front-end because it is widget-centric and by default uses a large amount of css and javascript. 

Nonetheless, **wt** is almost on par with **Flask**. The **Flask** built-in server is not asynchronous, and its speed suffers as a results. On the other hand, **wt** is based "on event-driven async I/O," and uses threads "to improve concurrent request handling". Evidently, **micro** and **wt** share very similar server logic. This brings into question why **wt** had such a high byte transfer rate compared to that of **micro**. That question is explored in the following experiment:    

###Static Test
In this test, we have each framework serve a static image, and nothing more.

<img src="../static.png" alt="Static" style="max-width: 70vw"/>

The results are interesting. During dynamic tests, **wt** was much faster than **micro** in terms of bytes per second. Yet during the static test, **wt** only beat **micro** by a small amount. That is, when **wt** and **micro** serve the same amount of static data, they operate at almost the same speed. This suggests that during dynamic testing, sending so much data over the network was not the speed bottleneck for **wt**. Indeed, during static testing, **wt** sends much more data in a shorter time. Therefore we hypothesize that in the dynamic case **wt** performed slower than **micro** because **wt** took longer to invoke the dynamic callback function.  

This is an important conclusion: **wt** was not slower than **micro** due to the large amount of data it was serving. Instead, it was slower than **micro** because it took longer for **wt** to respond to a dynamic request. Or, on the other hand, **micro** is highly optimized for responding to dynamic requests! 

###Optimizations for URL Resolution
Why is micro so fast? We hypothesize that it was due to our design decision to store **url_route**s in a vector. As explained in the documentation, a **url_route** is one of our custom classes that groups callback function together with a regex statement. When a client requests a url enpoint that matches that regex statement, the associated callback gets invoked. Matching a url endpoint takes place in the **url_route::match** member function. That is, the url_route checks itself to see if it can be matched with the supplied endpoint. The **url_routes** are iterated over linearly until a match is found.  

It is important that all of this is done in a vector. Because the **url_routes** are stored contiguously in memory, they are prefetched into the cache together. Thus, iterating over all of the url_routes is blazingly fast because the match function operates on objects sitting in cache. Earlier, we implemented the data structure for **url_route**s as an unordered map of regex expressions to url objects. This version was much slower - though we do not currently have the exact numbers. In the future, we hope to test our framework with the hashmap implementation to see if the vectorization of **url_route**s is really the source of speed in URL resolution.
