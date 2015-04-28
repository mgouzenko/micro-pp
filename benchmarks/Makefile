
micro_app_dynamic:
	clang++ -std=gnu++11 -lmicro -lboost_system -lboost_log-mt -lpthread micro_app_dynamic.cpp -o micro_app_dynamic
	./micro_app_dynamic.cpp


.PHONY: wt_app_dynamic
wt_app_dynamic:
	clang++ -lwt -lwthttp wt_app_dynamic.cpp -o wt_app_dynamic
	./hello --docroot . --http-address 0.0.0.0 --http-port 8080	
