# micro-pp

Micro++ is a tiny but fast, scalable and multithreaded web framework for C++.

Micro++ is a derivative work of the example ASIO HTTP Server 4 (Copyright (c) 2003-2011 Chris Kohlhoff)

Micro++ is thus distributed under the Boost Software License, Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt)

## Style Guidelines

* All classes and non-class-member functions should be in the micro:: namespace
* Soft tabs (4 spaces)
* Class fields should end in "\_"
* Header files should contain include guards in the format "\__NAMESPACE_FILENAME_HPP\__"
* Stroustrup indent style: http://en.wikipedia.org/wiki/Indent_style#Variant:_Stroustrup
* See Indentation, Whitespace, and Comments http://www.stroustrup.com/Programming/PPP-style-rev3.pdf
* Use Javadoc style comments when commenting on public methods and fields. The Doxygen is currently set up to parse Javadoc comments.

## Doxygen
Use Javadoc style comments when coding.

If you have doxygen installed on your system, generating the docs for this project is as simple as running

    doxygen

in this directory.

## Prerequisites for the library
To install and use the micro++ library, you will need the following packages installed:

    automake
    autoconf
    libtool
    boost

If you're on OS X, you can easily install all of these packages with homebrew. Micro was tested on OS X using clang++

On any Linux system, these packages are also easily installable with your distribution's package manager. Automake, autoconf, and libtool are all part of GNU autotools and boost is one of the most readily available C++ libraries. Micro was tested on Linux using g++

Micro++ was not tested on a Windows system.

If you're using a prepackaged version of the micro++ library with the configure script already included, the only preqrequisite is the `boost` library.

## Installing the library
The following four commands will install the micro++ library and its headers in /usr/local

    autoreconf -i
    ./configure
    make
    make install

Depending on your system configuration you may need to run make install with sudo/root.

When updating library code, run

    make uninstall && make && make install

If you are packaging the project for end user use, you can run `autoreconf -if` before packaging so the user will just have to run ./configure

## Using the library
To link micro++ with your project, use the following linker flags on Mac OS X:

    -lmicro -lboost_system -lboost_log-mt -lpthread

or the following linker flags on Linux

    -lmicro -lboost_system -lboost_log -lpthread

On Linux, you may also need to run the following command for the `-lmicro` flag to work.

    export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

Include this line in your .bashrc if you don't want to run this every time you reboot.

Your headers should be accessible in source files with macros like `#include <micro/app.hpp>`

Refer to the generated doxygen output and the example_webapps directory for details on how to build webapps using micro++. The Design Document and Tutorial can be found in the docs/ directory, as well as on the "Related Pages" tab in the Doxygen output.

Please note that the Makefiles included with the example_webapps are designed for use on OS X and may need to be edited if using Linux.
