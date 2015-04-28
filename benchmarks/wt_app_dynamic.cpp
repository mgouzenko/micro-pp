/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

// c++0x only, for std::bind
// #include <functional>

using namespace Wt;

/*
 * A simple hello world application class which demonstrates how to react
 * to events, read input, and give feed-back.
 */
class HelloApplication : public WApplication
{
public:
  HelloApplication(const WEnvironment& env);
};

/*
 * The env argument contains information about the new session, and
 * the initial request. It must be passed to the WApplication
 * constructor so it is typically also an argument for your custom
 * application constructor.
*/
HelloApplication::HelloApplication(const WEnvironment& env)
  : WApplication(env)
{
  const std::string none = "NONE"; 
  setTitle("Hello world");// application title
  const std::string *fname = env.getParameter("fname"); 
  const std::string *lname = env.getParameter("lname");
  const std::string *color = env.getParameter("color"); 
  
  if(fname == 0) fname = &none; 
  if(lname == 0) lname = &none;
  if(color == 0) color = &none; 

  root()->addWidget(new WText("Hello " + *fname + " " + *lname + ", your favorite color is " + *color+ "."));
}

WApplication *createApplication(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new HelloApplication(env);
}

int main(int argc, char **argv)
{
  /*
   * Your main method may set up some shared resources, but should then
   * start the server application (FastCGI or httpd) that starts listening
   * for requests, and handles all of the application life cycles.
   *
   * The last argument to WRun specifies the function that will instantiate
   * new application objects. That function is executed when a new user surfs
   * to the Wt application, and after the library has negotiated browser
   * support. The function should return a newly instantiated application
   * object.
   */
  return WRun(argc, argv, &createApplication);
}

