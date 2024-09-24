CC = tcc
CFLAGS = -Wall -std=89
SRC = lsystem.c siphash.c hashmap.c
PT_SRC = perftest.c test.c siphash.c hashmap.c

lsystem: $(SRC)
	$(CC) $(CFLAGS) -o lsystem $(SRC)

run:
	$(CC) $(CFLAGS) -o lsystem $(SRC)
	./lsystem

#test: lsystem.c
#	$(CC) $(CFLAGS) -DTEST -o lsystem lsystem.c siphash.c hashmap.c

perftest: $(PT_SRC)
	$(CC) $(CFLAGS) -DPERFTEST -o perftest $(PT_SRC)
	./perftest

clean:
	rm -f lsystem perftest
