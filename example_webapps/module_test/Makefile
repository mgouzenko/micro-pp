CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

module_test:

.PHONY: all
all: clean module_test

.PHONY: clean
clean:
	rm -f *.o  module_test > /dev/null