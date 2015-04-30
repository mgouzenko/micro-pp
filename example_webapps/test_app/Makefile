CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

test_app:

.PHONY: all
all: clean test_app

.PHONY: clean
clean:
	rm -f *.o  test_app > /dev/null