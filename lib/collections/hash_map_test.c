#include <assert.h>
#include <stdio.h>
#include "hash_map.h"

#define TEST_DATA_LEN(t) (sizeof((t))/sizeof(HashMapCCTestData))
#define ASSERT_MAP_ARR_ENTRY_EQ(m, i, k, v)                                    \
    assert((m).entry_arr[(i)].key == (k));                                     \
    assert((m).entry_arr[(i)].val == (v));                                     \
    assert((m).occ_arr[(i)/8] & 0x80 >> (i)%8);


typedef struct hash_map_cc_test_data {
    char key;
    char val;
    uint32 expected_index;
} HashMapCCTestData;

int count_occupied(HashMapCC *map) {
    int i;
    int count = 0;
    for (i = 0; i < map->size; ++i) {
        count += (map->occ_arr[i/8] >> i%8) & 0x01;
    }
    return count;
}

void hash_map_cc_insert_test() {
    HashMapCCTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 11 },
        { .key = 'b', .val = 'y', .expected_index = 12 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 14 },
    };

    HashMapCC map;
    hash_map_cc_create(&map, 16);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_insert(&map, test_data[i].key, test_data[i].val));
    }

    char out;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_get(&out, &map, test_data[i].key));
        assert(out == test_data[i].val);
        ASSERT_MAP_ARR_ENTRY_EQ(
                map,
                test_data[i].expected_index,
                test_data[i].key,
                test_data[i].val
        );
    }
    assert(map.count == 5);
    assert(map.size == 16);
    assert(count_occupied(&map) == map.count);

    hash_map_cc_destroy(&map);
}

void hash_map_cc_insert_resize_test() {
    HashMapCCTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 3 },
        { .key = 'b', .val = 'y', .expected_index = 4 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 6 },
    };

    HashMapCC map;
    hash_map_cc_create(&map, 4);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_insert(&map, test_data[i].key, test_data[i].val));
    }

    char out;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_get(&out, &map, test_data[i].key));
        assert(out == test_data[i].val);
    }
    assert(map.count == 5);
    assert(map.size == 8);
    assert(count_occupied(&map) == map.count);

    hash_map_cc_destroy(&map);
}

void hash_map_cc_insert_overwrite_test() {
    HashMapCCTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 11 },
        { .key = 'b', .val = 'y', .expected_index = 12 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 14 },
    };
    HashMapCCTestData unchanged_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 11 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
    };
    HashMapCCTestData overwrite_data[] = {
        { .key = 'b', .val = '1', .expected_index = 12 },
        { .key = 'd', .val = '2', .expected_index = 5 },
        { .key = 'e', .val = '3', .expected_index = 14 },
    };

    HashMapCC map;
    hash_map_cc_create(&map, 16);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_insert(&map, test_data[i].key, test_data[i].val));
    }
    for (i = 0; i < TEST_DATA_LEN(overwrite_data); ++i) {
        assert(!hash_map_cc_insert(
                    &map,
                    overwrite_data[i].key,
                    overwrite_data[i].val
        ));
    }

    char out;
    for (i = 0; i < TEST_DATA_LEN(unchanged_data); ++i) {
        assert(!hash_map_cc_get(&out, &map, unchanged_data[i].key));
        assert(out == unchanged_data[i].val);
        ASSERT_MAP_ARR_ENTRY_EQ(
                map,
                unchanged_data[i].expected_index,
                unchanged_data[i].key,
                unchanged_data[i].val
        );
    }
    for (i = 0; i < TEST_DATA_LEN(overwrite_data); ++i) {
        assert(!hash_map_cc_get(&out, &map, overwrite_data[i].key));
        assert(out == overwrite_data[i].val);
        ASSERT_MAP_ARR_ENTRY_EQ(
                map,
                overwrite_data[i].expected_index,
                overwrite_data[i].key,
                overwrite_data[i].val
        );
    }
    assert(map.count == 5);
    assert(map.size == 16);
    assert(count_occupied(&map) == map.count);

    hash_map_cc_destroy(&map);
}

void hash_map_cc_delete_test() {
    HashMapCCTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 11 },
        { .key = 'b', .val = 'y', .expected_index = 12 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 14 },
    };

    HashMapCC map;
    hash_map_cc_create(&map, 16);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_insert(&map, test_data[i].key, test_data[i].val));
    }

    char out;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_delete(&map, test_data[i].key));
        assert(hash_map_cc_get(&out, &map, test_data[i].key));
    }
    assert(map.count == 0);
    assert(map.size == 16);
    assert(count_occupied(&map) == map.count);

    hash_map_cc_destroy(&map);
}

void hash_map_cc_delete_resize_test() {
    HashMapCCTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 11 },
        { .key = 'b', .val = 'y', .expected_index = 12 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 14 },
    };

    HashMapCC map;
    hash_map_cc_create(&map, 4);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_insert(&map, test_data[i].key, test_data[i].val));
    }

    char out;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_delete(&map, test_data[i].key));
        assert(hash_map_cc_get(&out, &map, test_data[i].key));
    }
    assert(map.count == 0);
    assert(map.size == 8);
    assert(count_occupied(&map) == map.count);

    hash_map_cc_destroy(&map);
}

void hash_map_cc_delete_missing_test() {
    HashMapCCTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 11 },
        { .key = 'b', .val = 'y', .expected_index = 12 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 14 },
    };

    HashMapCC map;
    hash_map_cc_create(&map, 16);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_insert(&map, test_data[i].key, test_data[i].val));
    }

    assert(hash_map_cc_delete(&map, '1'));
    assert(hash_map_cc_delete(&map, '2'));
    assert(hash_map_cc_delete(&map, '3'));

    char out;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_cc_get(&out, &map, test_data[i].key));
        assert(out == test_data[i].val);
        ASSERT_MAP_ARR_ENTRY_EQ(
                map,
                test_data[i].expected_index,
                test_data[i].key,
                test_data[i].val
        );
    }
    assert(map.count == 5);
    assert(map.size == 16);
    assert(count_occupied(&map) == map.count);

    hash_map_cc_destroy(&map);
}

void hash_map_cc_max_size_test() {
    HashMapCC map;
    hash_map_cc_create(&map, 16);

    int i;
    for (i = 0; i < 256; ++i) {
        assert(!hash_map_cc_insert(&map, i, (i + 17)%256));
    }

    assert(map.count == 256);
    assert(map.size == 512);

    char out;
    for (i = 0; i < 256; ++i) {
        assert(!hash_map_cc_get(&out, &map, i));
        assert(out == (char)((i + 17)%256));
    }

    assert(map.count == 256);
    assert(map.size == 512);
    assert(count_occupied(&map) == map.count);

    for (i = 0; i < 256; ++i) {
        assert(!hash_map_cc_delete(&map, i));
        assert(hash_map_cc_get(&out, &map, i));
    }

    assert(map.count == 0);
    assert(map.size == 512);
    assert(count_occupied(&map) == map.count);

    hash_map_cc_destroy(&map);
}

int main() {
    hash_map_cc_insert_test();
    hash_map_cc_insert_resize_test();
    hash_map_cc_insert_overwrite_test();
    hash_map_cc_delete_test();
    hash_map_cc_delete_resize_test();
    hash_map_cc_delete_missing_test();
    hash_map_cc_max_size_test();
    printf("All tests passed!\n");
    return 0;
}
