CXX = clang++
CC = clang++               
LDLIBS = -lwt -lwthttp


wt_app: wt_app.o
wt_app.o:

.PHONY:clean
clean:
	rm -rf *.o wt_app > /dev/null 

.PHONY:run
run:
	./wt_app --docroot ../static --http-address 0.0.0.0 --http-port 8080	
