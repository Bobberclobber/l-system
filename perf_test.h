#ifndef PERF_TEST_H
#define PERF_TEST_H

#include "int_types.h"

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

#define START_TIMERS                                                           \
    clock_t c1 = clock();                                                      \
    time_t t1 = time(NULL);
#define END_TIMERS                                                             \
    clock_t c2 = clock();                                                      \
    time_t t2 = time(NULL);
#define PRINT_TIMES_AVG(d)                                                     \
    printf("    wall time (avg): %.2f\n", (float)difftime(t2, t1)/(d));        \
    printf("    cpu time (avg): %.2f\n", (float)(c2 - c1)/(d));
#define PRINT_COUNTER(i, l) printf("    %s: %ld\n", (l), ptd.counters[(i)]);
#define PRINT_COUNTER_AVG(i, d, l)                                             \
    printf(                                                                    \
            "    %s (avg): %ld (%.2f)\n",                                      \
            (l), ptd.counters[(i)],                                            \
            (float)ptd.counters[(i)]/(d)                                       \
    );

typedef struct perf_test_data {
    uint64 counters[PT_COUNTER_SIZE];
} PerfTestData;

void ptd_init(PerfTestData *ptd);

#endif
