CXX = clang++
CC = clang++
LDLIBS = -lboost_system
CXXFLAGS = -std=gnu++11 -DBOOST_LOG_DYN_LINK
ARFLAGS = rcs
AR = ar

libmicro.a: mime_types.o server.o request_parser.o reply.o response.o cookie.o \
	url_route.o request_handler.o app.o request.o micro_thread.o work_queue.o stop_watch.o
	$(AR) $(ARFLAGS) $@ $?

mime_types.o: mime_types.hpp
request_handler.o: request_handler.hpp server.hpp url_route.hpp request.hpp reply.hpp response.hpp
server.o: server.hpp work_queue.hpp request.hpp reply.hpp request_parser.hpp
request_parser.o: request_parser.hpp request_handler.hpp request.hpp
reply.o: reply.hpp header.hpp response.hpp
response.o: response.hpp header.hpp cookie.hpp
cookie.o: cookie.hpp
url_route.o: url_route.hpp request.hpp
app.o: app.hpp server.hpp work_queue.hpp micro_thread.hpp request_handler.hpp types.hpp url_route.hpp
request.o: request.hpp header.hpp
micro_thread.o: micro_thread.hpp stop_watch.hpp app.hpp reply.hpp header.hpp server.hpp 
work_queue.o: work_queue.hpp
stop_watch.o: stop_watch.hpp

.PHONY: clean
clean:
	rm *.o
