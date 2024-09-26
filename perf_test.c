#include <stdio.h>
#include <time.h>
#include "hash_map.h"

#define START_TIMERS                                                           \
    clock_t c1 = clock();                                                      \
    time_t t1 = time(NULL);
#define END_TIMERS                                                             \
    clock_t c2 = clock();                                                      \
    time_t t2 = time(NULL);
#define PRINT_TIMES_AVG(d)                                                     \
    printf("    wall time (avg): %.2f\n", difftime(t2, t1)/(d));               \
    printf("    cpu time (avg): %.2f\n", (c2 - c1)/(d));
#define PRINT_COUNTER(i, l) printf("    %s: %ld\n", (l), ptd.counters[(i)]);

void hash_map_create_destroy_ptest() {
    CHashMap map;
    int i;
    START_TIMERS;
    for (i = 0; i < 1000; ++i) {
        hash_map_create(&map, 1E9);
        hash_map_destroy(&map);
    }
    END_TIMERS;
    printf("PerfTest \"hash_map_create_destroy_ptest\" results:\n");
    PRINT_TIMES_AVG(1000.0);
    printf("\n");
}

void hash_map_insert_ptest() {
    PerfTestData ptd;
    ptd_init(&ptd);

    CHashMap map;
    hash_map_create(&map, 16);

    int i;
    START_TIMERS;
    for (i = 0; i < 256; ++i) {
        hash_map_insert(&ptd, &map, i, i);
    }
    END_TIMERS;

    hash_map_destroy(&map);

    printf("PerfTest \"hash_map_insert_ptest\" results:\n");
    PRINT_TIMES_AVG(256.0);
    PRINT_COUNTER(0, "num resizes");
    PRINT_COUNTER(1, "num inserts");
    PRINT_COUNTER(2, "num probes");
    PRINT_COUNTER(3, "num fails");
    printf("\n");
}

int main() {
    hash_map_create_destroy_ptest();
    hash_map_insert_ptest();
    return 0;
}
