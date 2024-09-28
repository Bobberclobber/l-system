#include <assert.h>
#include <stdio.h>
#include "hash_map.h"

#define TEST_DATA_LEN(t) sizeof((t))/sizeof(CHashMapTestData)
#define ASSERT_MAP_ARR_ENTRY_EQ(m, i, k, v)                                    \
    assert((m).entry_arr[(i)].key == (k));                                     \
    assert((m).entry_arr[(i)].val == (v));

typedef struct c_hash_map_test_data {
    char key;
    char val;
    uint32 expected_index;
} CHashMapTestData;

void hash_map_insert_success_test() {
    CHashMapTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 11 },
        { .key = 'b', .val = 'y', .expected_index = 12 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 14 },
    };

    CHashMap map;
    hash_map_create(&map, 16);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        hash_map_insert(&map, test_data[i].key, test_data[i].val);
    }

    char out;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        hash_map_get(&out, &map, test_data[i].key);
        assert(out == test_data[i].val);
        ASSERT_MAP_ARR_ENTRY_EQ(
                map,
                test_data[i].expected_index,
                test_data[i].key,
                test_data[i].val
        );
    }

    hash_map_destroy(&map);
}

int main() {
    hash_map_insert_success_test();
    printf("All tests passed!\n");
    return 0;
}
