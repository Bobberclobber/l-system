CC = tcc
CFLAGS = -Wall -std=89
SRC = lsystem.c siphash.c hash_map.c
T_SRC = hash_map_test.c siphash.c hash_map.c
PT_SRC = hash_map_perf_test.c perf_test.c siphash.c hash_map.c

lsystem: $(SRC)
	$(CC) $(CFLAGS) -o lsystem $(SRC)

run:
	$(CC) $(CFLAGS) -o lsystem $(SRC)
	./lsystem

test: $(T_SRC)
	$(CC) $(CFLAGS) -DTEST -o hash_map_test $(T_SRC)
	./hash_map_test

perf_test: $(PT_SRC)
	$(CC) $(CFLAGS) -DPERFTEST -o hash_map_perf_test $(PT_SRC)
	./hash_map_perf_test

clean:
	rm -f lsystem hash_map_perf_test hash_map_test
