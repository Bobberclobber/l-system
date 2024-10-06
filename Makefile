CC = tcc
CFLAGS = -Wall -std=c89
SRC = lsystem.c siphash.c hash_map.c
T_SRC = hash_map_test.c siphash.c hash_map.c
PT_SRC = hash_map_perf_test.c perf_test.c siphash.c hash_map.c

lsystem: $(SRC)
	$(CC) $(CFLAGS) -o lsystem.o $(SRC)

run:
	$(CC) $(CFLAGS) -o lsystem.o $(SRC)
	./lsystem.o

test: $(T_SRC)
	$(CC) $(CFLAGS) -DTEST -o hash_map_test.o $(T_SRC)
	./hash_map_test.o

perf_test: $(PT_SRC)
	$(CC) $(CFLAGS) -DPERFTEST -o hash_map_perf_test.o $(PT_SRC)
	./hash_map_perf_test.o

clean:
	rm -f *.o
