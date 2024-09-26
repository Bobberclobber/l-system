CC = tcc
CFLAGS = -Wall -std=89
SRC = lsystem.c siphash.c hash_map.c
PT_SRC = perf_test.c test.c siphash.c hash_map.c

lsystem: $(SRC)
	$(CC) $(CFLAGS) -o lsystem $(SRC)

run:
	$(CC) $(CFLAGS) -o lsystem $(SRC)
	./lsystem

#test: lsystem.c
#	$(CC) $(CFLAGS) -DTEST -o lsystem lsystem.c siphash.c hash_map.c

perf_test: $(PT_SRC)
	$(CC) $(CFLAGS) -DPERFTEST -o perf_test $(PT_SRC)
	./perf_test

clean:
	rm -f lsystem perf_test
