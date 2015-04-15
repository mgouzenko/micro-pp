CXX = clang++
CC = clang++
LDLIBS = -lboost_system -lboost_log-mt -lpthread -lmicro
LDFLAGS = -L../src
INC = -I../src
CXXFLAGS = -std=c++11 -I../src

test_app:

exception_test:

tests:

.PHONY: all 
all: clean exception_test test_app tests

.PHONY: clean
clean:
	rm -f *.o  exception_test test_app tests > /dev/null
