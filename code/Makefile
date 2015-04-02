CXX = g++
CC = g++
LDLIBS = -lboost_system
CXXFLAGS = -std=gnu++11


libmicro.a: app.o mime_types.o request_handler.o server.o request_parser.o reply.o response.o cookie.o url.o
	ar rcs libmicro.a app.o mime_types.o request_handler.o server.o request_parser.o reply.o response.o cookie.o url.o

mime_types.o: mime_types.hpp
request_handler.o: request_handler.hpp
server.o: server.hpp
request_parser.o: request_parser.hpp
reply.o: reply.hpp
app.o: app.hpp
<<<<<<< HEAD
response.o: response.hpp
cookie.o: cookie.hpp
=======
url.o: url.hpp
>>>>>>> 7673ff91210763e8e0f9750d79c33c66932ef028

.PHONY: clean
clean:
	rm *.o
