#include <stdio.h>
#include "hashmap.h"

int main() {
	printf("Running L-System..\n");

	CHashMap map;
	hash_map_create(&map, 8);

	hash_map_insert(&map, 'a', 'i');
	hash_map_insert(&map, 'b', 'j');
	hash_map_insert(&map, 'c', 'k');
	hash_map_insert(&map, 'd', 'l');
	hash_map_insert(&map, 'e', 'm');
	hash_map_insert(&map, 'f', 'n');
	hash_map_insert(&map, 't', 'o');
	hash_map_insert(&map, 'u', 'p');
	hash_map_insert(&map, 'v', 'q');
	hash_map_insert(&map, 'w', 'r');
	hash_map_insert(&map, 'x', 's');
	hash_map_insert(&map, 'y', 't');
	hash_map_insert(&map, 'z', 'u');

    printf("\n");

    int i;
	for (i = 0; i < map.size; ++i) {
		printf("%d: %c -> %c\n", i, map.entry_arr[i].key, map.entry_arr[i].val);
	}
	for (i = 0; i < 1 + (map.size - 1)/8; ++i) {
		printf("%08b\n", map.occ_arr[i]);
	}

    hash_map_destroy(&map);

	return 0;
}
