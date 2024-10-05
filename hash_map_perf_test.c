#include <stdio.h>
#include <time.h>
#include "hash_map.h"
#include "perf_test.h"

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
    PRINT_TIMES_AVG(1000);
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
    PRINT_TIMES_AVG(256);
    PRINT_COUNTER(0, "num resizes");
    PRINT_COUNTER_AVG(1, 256, "num inserts");
    PRINT_COUNTER_AVG(2, 256, "num probes");
    PRINT_COUNTER(3, "num fails");
    printf("\n");
}

void hash_map_delete_ptest() {
    PerfTestData ptd;
    ptd_init(&ptd);

    CHashMap map;
    hash_map_create(&map, 16);

    int i;
    for (i = 0; i < 256; ++i) {
        hash_map_insert(&ptd, &map, i, i);
    }

    ptd_init(&ptd);
    START_TIMERS;
    for (i = 0; i < 256; ++i) {
        hash_map_delete(&ptd, &map, i);
    }
    END_TIMERS;

    hash_map_destroy(&map);

    printf("PerfTest \"hash_map_delete_ptest\" results:\n");
    PRINT_TIMES_AVG(256);
    PRINT_COUNTER_AVG(0, 256, "num probes");
    PRINT_COUNTER(1, "num fails");
    printf("\n");
}

void hash_map_get_ptest() {
    PerfTestData ptd;
    ptd_init(&ptd);

    CHashMap map;
    hash_map_create(&map, 16);

    int i;
    for (i = 0; i < 256; ++i) {
        hash_map_insert(&ptd, &map, i, i);
    }

    ptd_init(&ptd);
    char out;
    START_TIMERS;
    for (i = 0; i < 256; ++i) {
        hash_map_get(&ptd, &out, &map, i);
    }
    END_TIMERS;

    hash_map_destroy(&map);

    printf("PerfTest \"hash_map_get_ptest\" results:\n");
    PRINT_TIMES_AVG(256);
    PRINT_COUNTER_AVG(0, 256, "num probes");
    PRINT_COUNTER(1, "num fails");
    printf("\n");
}

int main() {
    hash_map_create_destroy_ptest();
    hash_map_insert_ptest();
    hash_map_delete_ptest();
    hash_map_get_ptest();
    return 0;
}
