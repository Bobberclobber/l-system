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

    hash_map_cc_print(&map);

    hash_map_cc_destroy(&map);

	return 0;
}
