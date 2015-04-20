CXX = clang++
CC = clang++
LDLIBS = -lboost_system -lboost_log-mt -lpthread -lmicro
CXXFLAGS = -std=c++11

test_app:

exception_test:

timeout_test:

module_test:

tests:

.PHONY: all
all: clean exception_test test_app tests

.PHONY: clean
clean:
	rm -f *.o  exception_test test_app tests > /dev/null
