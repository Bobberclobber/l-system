#include <stdio.h>
#include "hash_map.h"

int main() {
	printf("Running L-System..\n");

	HashMapCC map;
	hash_map_cc_create(&map, 8);

	hash_map_cc_insert(&map, 'a', 'i');
	hash_map_cc_insert(&map, 'b', 'j');
	hash_map_cc_insert(&map, 'c', 'k');
	hash_map_cc_insert(&map, 'd', 'l');
	hash_map_cc_insert(&map, 'e', 'm');
	hash_map_cc_insert(&map, 'f', 'n');
	hash_map_cc_insert(&map, 't', 'o');
	hash_map_cc_insert(&map, 'u', 'p');
	hash_map_cc_insert(&map, 'v', 'q');
	hash_map_cc_insert(&map, 'w', 'r');
	hash_map_cc_insert(&map, 'x', 's');
	hash_map_cc_insert(&map, 'y', 't');
	hash_map_cc_insert(&map, 'z', 'u');

    printf("\n");

    int i;
	for (i = 0; i < map.size; ++i) {
		printf("%d: %c -> %c\n", i, map.entry_arr[i].key, map.entry_arr[i].val);
	}
	for (i = 0; i < 1 + (map.size - 1)/8; ++i) {
		printf("%08b\n", map.occ_arr[i]);
	}

    hash_map_cc_destroy(&map);

	return 0;
}
