#ifndef HASHMAP_H
#define HASHMAP_H

#include "int_types.h"
#include "perf_test.h"

typedef struct hash_map_cc_entry {
    char key;
    char val;
} HashMapCCEntry;

typedef struct hash_map_cc {
	uint32 count;
	uint32 size;
	HashMapCCEntry *entry_arr;
	uint8 *occ_arr;
	char k[16];
} HashMapCC;

/*
 * TODO: Standardize and formalize doc strings.
 * *map: pointer to new CHashMap.
 * init_size: The initial maximum number of elements that the map can hold.
 *            This should ideally be a power of 2.
 */
void hash_map_create(HashMapCC *map, uint32 init_size);
void hash_map_destroy(HashMapCC *map);
int hash_map_insert(PT_ENABLED HashMapCC *map, char key, char val);
int hash_map_delete(PT_ENABLED HashMapCC *map, char key);
int hash_map_get(PT_ENABLED char *out, HashMapCC *map, char key);
void hash_map_print(HashMapCC *map);

#endif
