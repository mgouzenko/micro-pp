CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

module_example:

.PHONY: all
all: clean module_example 

.PHONY: clean
clean:
	rm -f *.o  module_example > /dev/null
