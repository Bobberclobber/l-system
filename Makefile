CC = tcc
CFLAGS = -Wall -std=c89
SRC = lsystem.c siphash.c hash_map.c
T_SRC = hash_map_test.c siphash.c hash_map.c
PT_SRC = hash_map_perf_test.c perf_test.c siphash.c hash_map.c

lsystem: $(SRC)
	$(CC) $(CFLAGS) -o lsystem.exe $(SRC)

run:
	$(CC) $(CFLAGS) -o lsystem.exe $(SRC)
	./lsystem.exe

test: $(T_SRC)
	$(CC) $(CFLAGS) -DTEST -o hash_map_test.exe $(T_SRC)
	./hash_map_test.exe

perf_test: $(PT_SRC)
	$(CC) $(CFLAGS) -DPERFTEST -o hash_map_perf_test.exe $(PT_SRC)
	./hash_map_perf_test.exe

clean:
	rm -f *.exe
