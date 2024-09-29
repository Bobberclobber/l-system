#include <assert.h>
#include <stdio.h>
#include "hash_map.h"

#define TEST_DATA_LEN(t) (sizeof((t))/sizeof(CHashMapTestData))
#define ASSERT_MAP_ARR_ENTRY_EQ(m, i, k, v)                                    \
    assert((m).entry_arr[(i)].key == (k));                                     \
    assert((m).entry_arr[(i)].val == (v));                                     \
    assert((m).occ_arr[(i)/8] & 0x80 >> (i)%8);


typedef struct c_hash_map_test_data {
    char key;
    char val;
    uint32 expected_index;
} CHashMapTestData;

int count_occupied(CHashMap *map) {
    int i;
    int count = 0;
    for (i = 0; i < map->size; ++i) {
        count += (map->occ_arr[i/8] >> i%8) & 0x01;
    }
    return count;
}

void c_hash_map_insert_test() {
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
        assert(!hash_map_get(&out, &map, test_data[i].key));
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

    hash_map_destroy(&map);
}

void c_hash_map_insert_resize_test() {
    CHashMapTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 3 },
        { .key = 'b', .val = 'y', .expected_index = 4 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 6 },
    };

    CHashMap map;
    hash_map_create(&map, 4);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        hash_map_insert(&map, test_data[i].key, test_data[i].val);
    }

    char out;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_get(&out, &map, test_data[i].key));
        assert(out == test_data[i].val);
    }
    assert(map.count == 5);
    assert(map.size == 8);
    assert(count_occupied(&map) == map.count);

    hash_map_destroy(&map);
}

void c_hash_map_delete_test() {
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
        assert(!hash_map_delete(&map, test_data[i].key));
        assert(hash_map_get(&out, &map, test_data[i].key));
    }
    assert(map.count == 0);
    assert(map.size == 16);
    assert(count_occupied(&map) == map.count);

    hash_map_destroy(&map);
}

void c_hash_map_delete_resize_test() {
    CHashMapTestData test_data[] = {
        { .key = 'a', .val = 'x', .expected_index = 11 },
        { .key = 'b', .val = 'y', .expected_index = 12 },
        { .key = 'c', .val = 'z', .expected_index = 0 },
        { .key = 'd', .val = 'v', .expected_index = 5 },
        { .key = 'e', .val = 'w', .expected_index = 14 },
    };

    CHashMap map;
    hash_map_create(&map, 4);

    int i;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        hash_map_insert(&map, test_data[i].key, test_data[i].val);
    }

    char out;
    for (i = 0; i < TEST_DATA_LEN(test_data); ++i) {
        assert(!hash_map_delete(&map, test_data[i].key));
        assert(hash_map_get(&out, &map, test_data[i].key));
    }
    assert(map.count == 0);
    assert(map.size == 8);
    assert(count_occupied(&map) == map.count);

    hash_map_destroy(&map);
}

void c_hash_map_max_size_test() {
    CHashMap map;
    hash_map_create(&map, 16);

    int i;
    for (i = 0; i < 256; ++i) {
        assert(!hash_map_insert(&map, i, (i + 17)%256));
    }

    assert(map.count == 256);
    assert(map.size == 512);

    char out;
    for (i = 0; i < 256; ++i) {
        assert(!hash_map_get(&out, &map, i));
        assert(out == (char)((i + 17)%256));
    }

    assert(map.count == 256);
    assert(map.size == 512);
    assert(count_occupied(&map) == map.count);

    for (i = 0; i < 256; ++i) {
        assert(!hash_map_delete(&map, i));
        assert(hash_map_get(&out, &map, i));
    }

    assert(map.count == 0);
    assert(map.size == 512);
    assert(count_occupied(&map) == map.count);

    hash_map_destroy(&map);
}

int main() {
    c_hash_map_insert_test();
    c_hash_map_insert_resize_test();
    /* c_hash_map_insert_overwrite_test(); */
    c_hash_map_delete_test();
    c_hash_map_delete_resize_test();
    /* c_hash_map_delete_missing_test(); */
    c_hash_map_max_size_test();
    printf("All tests passed!\n");
    return 0;
}
