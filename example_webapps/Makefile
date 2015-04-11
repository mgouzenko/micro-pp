CXX = clang++
CC = clang++
LDLIBS = -lboost_system -lboost_log-mt -lpthread -lmicro
LDFLAGS = -L../src
INC = -I../src
CXXFLAGS = -std=c++11 -I../src

test_app:

exception_test:

.PHONY: all 
all: clean exception_test test_app

.PHONY: clean
clean:
	rm -f *.o  exception_test test_app > /dev/null
