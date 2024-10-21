CC = gcc
CFLAGS = -Wall -std=c89 -z noexecstack
PWD = $(shell pwd)
SRC = hash_map.c
SRC_T = *_test.c $(SRC)
SRC_PT = *_test_perf.c $(SRC)
INC = -I$(PWD)/../types -I$(PWD)/../hashing -I$(PWD)/../perf_testing
LIB = -L$(PWD)/../hashing -L$(PWD)/../perf_testing -lhashing -lperf_testing

collections: $(SRC)
	$(CC) $(CFLAGS) $(INC) -c -o hash_map.o $(SRC) $(LIB)
	ar rcs libcollections.a *.o

test: $(SRC_T)
	$(CC) $(CFLAGS) $(INC) -DTEST -static -o hash_map_test.exe $(SRC_T) $(LIB)
	./hash_map_test.exe

perf_test: $(SRC_PT)
	$(CC) $(CFLAGS) $(INC) -DPERFTEST -o hash_map_perf_test.exe $(SRC_PT) $(LIB)
	./hash_map_perf_test.exe

clean:
	rm -f *.a *.o *.so *.exe
