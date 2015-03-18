CXX = g++ 
CC = g++

LDLIBS = -lboost_system

main: main.o mime_types.o file_handler.o server.o request_parser.o reply.o

.PHONY: clean
clean:
	rm *.o
