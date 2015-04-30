CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

timeout_test:

.PHONY: all
all: clean timeout_test

.PHONY: clean
clean:
	rm -f *.o  timeout_test > /dev/null