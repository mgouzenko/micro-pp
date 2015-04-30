CXX = clang++
CC = clang++
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread
CXXFLAGS = -std=gnu++11

unit_test_app:

.PHONY: all
all: clean unit_test_app

.PHONY: clean
clean:
	rm -f *.o  unit_test_app > /dev/null
