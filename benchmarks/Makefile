CXX = clang++
CC = clang++               
LDLIBS = -lmicro -lboost_system -lboost_log-mt -lpthread -lwt -lwthttp
CXXFLAGS = -std=gnu++11

all: micro_app_dynamic wt_app_dynamic
micro_app_dynamic: micro_app_dynamic.o
micro_app_dynamic.o:

.PHONY: wt_app_dynamic
wt_app_dynamic:
	clang++ wt_app_dynamic.cpp -lwt -lwthttp -o wt_app_dynamic

.PHONY:run_wt_dynamic
run_wt_dynamic: 
	./wt_app_dynamic --docroot . --http-address 0.0.0.0 --http-port 8080	
