#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "inttypes.h"
#include "siphash.h"
#include "test.h"

#define HASH_LEN 8
#define CHAR_BYTE_LEN 1

#define U8TO64_BE(p)                                                           \
    (((uint64)((p)[7])) | ((uint64)((p)[6]) << 8) |                            \
     ((uint64)((p)[5]) << 16) |((uint64)((p)[4]) << 24) |                      \
     ((uint64)((p)[3]) << 32) |((uint64)((p)[2]) << 40) |                      \
     ((uint64)((p)[1]) << 48) |((uint64)((p)[0]) << 56))

#define OCC_SIZE(s) 1 + ((s) - 1)/8
#define IS_OCCUPIED(a, b) (a)[(b)/8] & 0x80 >> (b)%8
#define SET_OCCUPIED(a, b) (a)[(b)/8] |= (0x80 >> (b)%8)
#define OFFSET_BUCKET(b, o) ((b) + (o))%map->size


void hash_map_create(CHashMap *map, uint32 init_size) {
    /* TODO: Assert against uint32 max. */
    assert(init_size != 0);
    map->count = 0;
    map->size = init_size;
    map->entry_arr = malloc(init_size * sizeof(CHashMapEntry));
    map->occ_arr = calloc(OCC_SIZE(init_size), 1);
    int i;
    for (i = 0; i < 16; ++i) {
        map->k[i] = i;
    }
}

void hash_map_destroy(CHashMap *map) {
    free(map->entry_arr);
    free(map->occ_arr);
}

int hash_map_insert_internal(PT_ENABLED CHashMap *map, char key, char val) {
    PT_COUNTER_INC(1, 1);
    uint8 out[HASH_LEN];
    siphash(&key, CHAR_BYTE_LEN, map->k, out, HASH_LEN);
    uint32 bucket = U8TO64_BE(out) % map->size;
    uint32 offset = 0;
    while (offset < map->size &&
           IS_OCCUPIED(map->occ_arr, OFFSET_BUCKET(bucket, offset)) &&
           map->entry_arr[OFFSET_BUCKET(bucket, offset)].key != key) {
        offset += 1;
    }
    PT_COUNTER_INC(2, offset);
    if (offset >= map->size) {
        PT_COUNTER_INC(3, 1);
        return 1;
    }
    bucket = OFFSET_BUCKET(bucket, offset);
    map->entry_arr[bucket] = (CHashMapEntry) { .key = key, .val = val };
    SET_OCCUPIED(map->occ_arr, bucket);
    map->count += 1;
    return 0;
}

int hash_map_insert(PT_ENABLED CHashMap *map, char key, char val) {
    if (map->count > (map->size - (map->size >> 2))) {
        PT_COUNTER_INC(0, 1);
        uint32 old_size = map->size;
        CHashMapEntry *old_entries = map->entry_arr;
        uint8 *old_occ = map->occ_arr;
        hash_map_create(map, map->size << 1);
        int i;
        for (i = 0; i < old_size; ++i) {
            if (IS_OCCUPIED(old_occ, i)) {
                int res = hash_map_insert_internal(
                        PT_INPUT
                        map,
                        old_entries[i].key,
                        old_entries[i].val
                );
                if (res) {
                    return res;
                }
            }
        }
        free(old_entries);
        free(old_occ);
    }
    return hash_map_insert_internal(PT_INPUT map, key, val);
}
