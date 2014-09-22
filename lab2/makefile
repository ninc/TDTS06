OS = $(shell uname -s)

CC=gccfilter -c -a g++
CFLAGS= -std=c++11 -c -Wpedantic -Wall -Wextra -Werror
LDFLAGS= -L/sw/gcc-4.9.1/lib -lnsl -lsocket -lresolv


ifeq ($(OS), Linux)
	CC= g++
	CFLAGS= -std=c++11 -c -Wpedantic -Wall -Wextra -Werror
	LDFLAGS= -L/sw/gcc-4.9.1/lib
endif


all: msp

msp: main.o socket_server.o socket_client.o url_filter.o cache.o content_filter.o
	$(CC) main.o socket_server.o socket_client.o url_filter.o cache.o content_filter.o $(LDFLAGS) -o msp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

socket_server.o: socket_server.cpp
	$(CC) $(CFLAGS) socket_server.cpp

socket_client.o: socket_client.cpp
	$(CC) $(CFLAGS) socket_client.cpp	

url_filter.o: url_filter.cpp
	$(CC) $(CFLAGS) url_filter.cpp

cache.o: cache.cpp
	$(CC) $(CFLAGS) cache.cpp

content_filter.o : content_filter.cpp
	$(CC) $(CFLAGS) content_filter.cpp		

clean:
	rm -rf *o msp
