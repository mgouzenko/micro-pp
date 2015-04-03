CXX = clang++
CC = clang++
LDLIBS = -lboost_system
CXXFLAGS = -std=gnu++11


.PHONY:test_app
test_app:
	clang++ -std=gnu++11 -I../src test_app.cpp -lboost_system -L../src -lmicro  -o test_app

.PHONY: clean
clean:
	rm *.o test_app
