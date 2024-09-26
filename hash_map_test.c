#include <assert.h>
#include "hash_map.h"

#define ASSERT_MAP_ENTRY_EQ(e, k, v)                                           \
    assert((e).key == (k));                                                    \
    assert((e).val == (v));

void hash_map_insert_success_test() {
    CHashMap map;
    hash_map_create(&map, 16);

    hash_map_insert(&map, 'a', 'x');
    hash_map_insert(&map, 'b', 'y');
    hash_map_insert(&map, 'c', 'z');
    hash_map_insert(&map, 'd', 'v');
    hash_map_insert(&map, 'e', 'w');

    hash_map_print(&map);

    ASSERT_MAP_ENTRY_EQ(map.entry_arr[0], 'c', 'z');
    ASSERT_MAP_ENTRY_EQ(map.entry_arr[5], 'd', 'v');
    ASSERT_MAP_ENTRY_EQ(map.entry_arr[11], 'a', 'x');
    ASSERT_MAP_ENTRY_EQ(map.entry_arr[12], 'b', 'y');
    ASSERT_MAP_ENTRY_EQ(map.entry_arr[14], 'e', 'w');

    hash_map_destroy(&map);
}

int main() {
    hash_map_insert_success_test();
    return 0;
}
