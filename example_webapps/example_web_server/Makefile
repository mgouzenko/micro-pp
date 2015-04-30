CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

web_server:

.PHONY: all
all: clean web_server

.PHONY: clean
clean:
	rm -f *.o  web_server > /dev/null