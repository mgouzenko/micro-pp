CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

exception_test:

.PHONY: all
all: clean exception_test

.PHONY: clean
clean:
	rm -f *.o  exception_test > /dev/null