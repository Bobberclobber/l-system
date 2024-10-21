CC = gcc
CFLAGS = -Wall -std=c89
PWD = $(shell pwd)
SRC = *.c
INC = -I$(PWD)/../types

perf_testing: $(SRC)
	$(CC) $(CFLAGS) $(INC) -c -o perf_testing.o $(SRC)
	ar rcs libperf_testing.a *.o

clean:
	rm -f *.a *.o
