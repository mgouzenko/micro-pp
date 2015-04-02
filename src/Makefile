CXX = clang++
CC = clang++
LDLIBS = -lboost_system
CXXFLAGS = -std=gnu++11


libmicro.a: app.o mime_types.o request_handler.o server.o request_parser.o reply.o response.o cookie.o url_route.o
	ar rcs libmicro.a app.o mime_types.o request_handler.o server.o request_parser.o reply.o response.o cookie.o url_route.o

mime_types.o: mime_types.hpp
request_handler.o: request_handler.hpp
server.o: server.hpp
request_parser.o: request_parser.hpp
reply.o: reply.hpp
app.o: app.hpp server.hpp
response.o: response.hpp
cookie.o: cookie.hpp
url_route.o: url_route.hpp

.PHONY: clean
clean:
	rm *.o
