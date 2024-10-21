#include "perf_test.h"

void ptd_init(PerfTestData *ptd) {
    int i;
    for (i = 0; i < PT_COUNTER_SIZE; ++i) {
        ptd->counters[i] = 0;
    }
}
