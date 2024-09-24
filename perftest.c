#include <stdio.h>
#include <time.h>
#include "hashmap.h"

#define NUM_ITER_I 1000
#define NUM_ITER_F 1000.0

void hashmap_create_destroy_ptest() {
    CHashMap map;
    int i;
    clock_t c1 = clock();
    time_t t1 = time(NULL);
    for (i = 0; i < NUM_ITER_I; ++i) {
        hash_map_create(&map, 1E9);
        hash_map_destroy(&map);
    }
    clock_t c2 = clock();
    time_t t2 = time(NULL);
    printf("PerfTest \"hashmap_create_destroy_ptest\" results:\n");
    printf("    wall time (avg): %.2f\n", difftime(t2, t1)/NUM_ITER_F);
    printf("     cpu time (avg): %.2f\n", (c2 - c1)/NUM_ITER_F);
    printf("\n");
}

void hashmap_insert_ptest() {
    PerfTestData ptd;
    ptd_init(&ptd);

    CHashMap map;
    hash_map_create(&map, 16);

    int i;
    for (i = 0; i < 256; ++i) {
        hash_map_insert(&ptd, &map, i, i);
    }

    hash_map_destroy(&map);

    printf("PerfTest \"hashmap_insert_ptest\" results:\n");
    printf("    num resizes: %ld\n", ptd.counters[0]);
    printf("    num inserts: %ld\n", ptd.counters[1]);
    printf("     num probes: %ld\n", ptd.counters[2]);
    printf("      num fails: %ld\n", ptd.counters[3]);
    printf("\n");
}

int main() {
    hashmap_create_destroy_ptest();
    hashmap_insert_ptest();
    return 0;
}
