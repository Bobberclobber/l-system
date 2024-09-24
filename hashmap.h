#ifndef HASHMAP_H
#define HASHMAP_H

#include "inttypes.h"
#include "test.h"

typedef struct c_hash_map_entry {
    char key;
    char val;
} CHashMapEntry;

typedef struct c_hash_map {
	uint32 count;
	uint32 size;
	CHashMapEntry *entry_arr;
	uint8 *occ_arr;
	char k[16];
} CHashMap;

/*
 * TODO: Standardize and formalize doc strings.
 * *map: pointer to new CHashMap.
 * init_size: The initial maximum number of elements that the map can hold.
 *            This should ideally be a power of 2.
 */
void hash_map_create(CHashMap *map, uint32 init_size);
void hash_map_destroy(CHashMap *map);
int hash_map_insert(PT_ENABLED CHashMap *map, char key, char val);

#endif
