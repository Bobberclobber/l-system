CC = gcc
CFLAGS = -Wall -std=c89
PWD = $(shell pwd)
SRC = *.c
INC = -I$(PWD)/../types

hashing: $(SRC)
	$(CC) $(CFLAGS) $(INC) -c -o siphash.o $(SRC)
	ar rcs libhashing.a *.o

clean:
	rm -f *.a *.o
