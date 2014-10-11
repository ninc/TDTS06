OS = $(shell uname -s)

CC= g++
CFLAGS= -std=c++11 -c -Wpedantic -Wall -Wextra -Werror
LDFLAGS= -L/sw/gcc-4.9.1/lib -lnsl -lsocket -lresolv


ifeq ($(OS), Linux)
	CC= g++
	CFLAGS= -std=c++11 -c -Wpedantic -Wall -Wextra -Werror
	LDFLAGS= -L/sw/gcc-4.9.1/lib
endif


all: msc

msc: main.o cache.o
	$(CC) main.o cache.o $(LDFLAGS) -o msc

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

cache.o: cache.cpp
	$(CC) $(CFLAGS) cache.cpp		

clean:
	rm -rf *o msc *~