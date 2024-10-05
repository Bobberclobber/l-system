#ifndef HASHMAP_H
#define HASHMAP_H

#include "int_types.h"
#include "perf_test.h"

#define DECLARE_HASH_MAP_TYPE(n, a, kt, vt)                                     \
    typedef struct n ## _entry {                                               \
        kt key;                                                                \
        vt val;                                                                \
    } a ## Entry;                                                              \
                                                                               \
    typedef struct n {                                                         \
    	uint32 count;                                                          \
    	uint32 size;                                                           \
    	a ## Entry *entry_arr;                                                 \
    	uint8 *occ_arr;                                                        \
    	char k[16];                                                            \
    } a;                                                                       \
                                                                               \
    /*                                                                         \
     * Allocate memory for values and initialize meta data fields.             \
     *                                                                         \
     *      *map: Pointer to new hash map.                                     \
     * init_size: The initial number of elements that the map can hold. This   \
     *            should ideally be a power of 2.                              \
     */                                                                        \
    void n ## _create(a *map, uint32 init_size);                               \
    void n ## _destroy(a *map);                                                \
    int n ## _insert(PT_ENABLED a *map, kt key, vt val);                       \
    int n ## _delete(PT_ENABLED a *map, kt key);                               \
    int n ## _get(PT_ENABLED vt *out, a *map, kt key);                         \
    void n ## _print(a *map);

DECLARE_HASH_MAP_TYPE(hash_map_cc, HashMapCC, char, char);
DECLARE_HASH_MAP_TYPE(hash_map_ci, HashMapCI, char, int);

#endif
