CXX = clang++
LDLIBS = -lboost_system
CXXFLAGS = -std=gnu++11


url_demo: url_demo.cpp
	$(CXX) -std=gnu++11 -I../src url_demo.cpp -lboost_system -L../src -lmicro  -o url_demo

.PHONY: clean
clean:
	rm url_demo
