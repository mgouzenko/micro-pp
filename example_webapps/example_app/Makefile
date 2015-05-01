CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

example_app:

.PHONY: all
all: clean example_app

.PHONY: clean
clean:
	rm -f *.o  example > /dev/null