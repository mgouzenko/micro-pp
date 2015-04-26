# micro-pp

A tiny but fast, scalable and multithreaded web framework for C++.

Micro++ is a derivative work of the example ASIO HTTP Server 4 (Copyright (c) 2003-2011 Chris Kohlhoff)

Micro++ is thus distributed under the Boost Software License, Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt)

## Style Guidelines

* All classes and non-class-member functions should be in the micro:: namespace
* Soft tabs (4 spaces)
* Class fields should end in "_"
* Header files should contain include guards in the format "\__NAMESPACE_FILENAME_HPP\__"
* Stroustrup indent style: http://en.wikipedia.org/wiki/Indent_style#Variant:_Stroustrup
* See Indentation, Whitespace, and Comments http://www.stroustrup.com/Programming/PPP-style-rev3.pdf
* Use Javadoc style comments when commenting on public methods and fields. The Doxygen is currently set up to parse Javadoc comments.

## Doxygen
Use Javadoc style comments when coding.

If you have doxygen installed on your system, generating the docs for this project is as simple as running
```
  doxygen
```
in this directory.

## Installing and using the library
The following four commands will install the library and its headers in /usr/local so you can link to it from any project with -lmicro and access its headers with #include statements like #include <micro/app.hpp>

```
    autoreconf -i
    ./configure
    make
    make install
```

When updating library code, run
```
    make uninstall && make && make install
```

If you are packaging the project for end user use, you can run autoreconf -i before packaging so the user will just have to run ./configure
