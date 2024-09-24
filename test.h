#ifndef TEST_H
#define TEST_H

#include "inttypes.h"

#define PT_COUNTER_SIZE 16

#ifndef PERFTEST
#define PT_ENABLED
#define PT_INPUT
#define PT_COUNTER_INC(i, x)
#endif

#ifdef PERFTEST
#define PT_ENABLED PerfTestData *ptd,
#define PT_INPUT ptd,
#define PT_COUNTER_INC(i, x)                                                   \
    if (0 <= (i) && (i) < PT_COUNTER_SIZE) {                                   \
        ptd->counters[(i)] += (x);                                             \
    } else {                                                                   \
        printf("PT counter %d at line %d out of range.", (i), __LINE__);       \
    }
#endif

typedef struct perf_test_data {
    uint64 counters[PT_COUNTER_SIZE];
} PerfTestData;

void ptd_init(PerfTestData *ptd);

#endif
