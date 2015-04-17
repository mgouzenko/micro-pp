CXX = clang++
CC = clang++
LDLIBS = -lboost_system
CXXFLAGS = -std=gnu++11 -DBOOST_LOG_DYN_LINK
ARFLAGS = rcs

libmicro.a: mime_types.o server.o request_parser.o reply.o response.o cookie.o url_route.o request_handler.o app.o request.o micro_thread.o
	$(AR) $(ARFLAGS) $@ $?
mime_types.o: mime_types.hpp
request_handler.o: request_handler.hpp
server.o: server.hpp work_queue.hpp
request_parser.o: request_parser.hpp
reply.o: reply.hpp
response.o: response.hpp
cookie.o: cookie.hpp
url_route.o: url_route.hpp
app.o: app.hpp server.hpp work_queue.hpp
request.o: request.hpp
micro_thread.o: micro_thread.hpp

.PHONY: clean
clean:
	rm *.o
