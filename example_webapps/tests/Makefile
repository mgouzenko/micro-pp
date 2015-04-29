CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

tests:

.PHONY: all
all: clean tests

.PHONY: clean
clean:
	rm -f *.o  tests > /dev/null