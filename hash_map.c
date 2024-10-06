#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash_map.h"
#include "int_types.h"
#include "siphash.h"
#include "test.h"

#define HASH_LEN 8

#define U8TO64_BE(p)                                                           \
    (((uint64)((p)[7])) | ((uint64)((p)[6]) << 8) |                            \
     ((uint64)((p)[5]) << 16) |((uint64)((p)[4]) << 24) |                      \
     ((uint64)((p)[3]) << 32) |((uint64)((p)[2]) << 40) |                      \
     ((uint64)((p)[1]) << 48) |((uint64)((p)[0]) << 56))

#define OCC_SIZE(s) (1 + ((s) - 1)/8)
#define IS_OCCUPIED(m, b) ((m).occ_arr[(b)/8] & 0x80 >> (b)%8)
#define SET_OCCUPIED(m, b) ((m).occ_arr[(b)/8] |= (0x80 >> (b)%8))
#define SET_UNOCCUPIED(m, b) ((m).occ_arr[(b)/8] &= ~(0x80 >> (b)%8))
#define OFFSET_BUCKET(b, o) (((b) + (o))%map->size)

#define DEFINE_HASH_MAP_CREATE(n, a)                                           \
    void n ## _create(a *map, uint32 init_size) {                              \
        /* TODO: Assert against uint32 max. */                                 \
        assert(init_size != 0);                                                \
        map->count = 0;                                                        \
        map->size = init_size;                                                 \
        map->entry_arr = malloc(init_size * sizeof(a ## Entry));               \
        map->occ_arr = calloc(OCC_SIZE(init_size), 1);                         \
        int i;                                                                 \
        for (i = 0; i < 16; ++i) {                                             \
            map->k[i] = i;                                                     \
        }                                                                      \
    }

#define DEFINE_HASH_MAP_DESTROY(n, a)                                          \
    void n ## _destroy(a *map) {                                               \
        free(map->entry_arr);                                                  \
        free(map->occ_arr);                                                    \
    }

#define DEFINE_HASH_MAP_INSERT(n, a, kt, vt)                                   \
    int n ## _insert_internal(PT_ENABLED a *map, kt key, vt val) {             \
        PT_COUNTER_INC(1, 1);                                                  \
        uint8 hash_out[HASH_LEN];                                              \
        siphash(&key, sizeof(kt), map->k, hash_out, HASH_LEN);                 \
        uint32 bucket = U8TO64_BE(hash_out) % map->size;                       \
        uint32 offset = 0;                                                     \
        while (offset < map->size &&                                           \
               IS_OCCUPIED(*map, OFFSET_BUCKET(bucket, offset)) &&             \
               map->entry_arr[OFFSET_BUCKET(bucket, offset)].key != key) {     \
            offset += 1;                                                       \
        }                                                                      \
        PT_COUNTER_INC(2, offset);                                             \
        if (offset >= map->size) {                                             \
            PT_COUNTER_INC(3, 1);                                              \
            return 1;                                                          \
        }                                                                      \
        bucket = OFFSET_BUCKET(bucket, offset);                                \
        map->entry_arr[bucket] = (a ## Entry) { .key = key, .val = val };      \
        if (!IS_OCCUPIED(*map, bucket)) {                                      \
            SET_OCCUPIED(*map, bucket);                                        \
            map->count += 1;                                                   \
        }                                                                      \
        return 0;                                                              \
    }                                                                          \
                                                                               \
    int n ## _insert(PT_ENABLED a *map, kt key, vt val) {                      \
        /* Resize if the map is ~75% full. */                                  \
        if (map->count > (map->size - (map->size >> 2))) {                     \
            PT_COUNTER_INC(0, 1);                                              \
            a old_map = *map;                                                  \
            n ## _create(map, map->size << 1);                                 \
            int i;                                                             \
            for (i = 0; i < old_map.size; ++i) {                               \
                if (IS_OCCUPIED(old_map, i)) {                                 \
                    int res = n ## _insert_internal(                           \
                            PT_INPUT                                           \
                            map,                                               \
                            old_map.entry_arr[i].key,                          \
                            old_map.entry_arr[i].val                           \
                    );                                                         \
                    if (res) {                                                 \
                        return res;                                            \
                    }                                                          \
                }                                                              \
            }                                                                  \
            n ## _destroy(&old_map);                                           \
        }                                                                      \
        return n ## _insert_internal(PT_INPUT map, key, val);                  \
    }

#define DEFINE_HASH_MAP_DELETE(n, a, kt)                                       \
    int n ## _delete(PT_ENABLED a *map, kt key) {                              \
        uint8 hash_out[HASH_LEN];                                              \
        siphash(&key, sizeof(kt), map->k, hash_out, HASH_LEN);                 \
        uint32 bucket = U8TO64_BE(hash_out) % map->size;                       \
        uint32 offset = 0;                                                     \
        while (offset < map->size) {                                           \
            PT_COUNTER_INC(0, 1);                                              \
            if (IS_OCCUPIED(*map, OFFSET_BUCKET(bucket, offset)) &&            \
                map->entry_arr[OFFSET_BUCKET(bucket, offset)].key == key) {    \
                SET_UNOCCUPIED(*map, OFFSET_BUCKET(bucket, offset));           \
                map->count -= 1;                                               \
                return 0;                                                      \
            }                                                                  \
            offset += 1;                                                       \
        }                                                                      \
        PT_COUNTER_INC(1, 1);                                                  \
        return 1;                                                              \
    }

#define DEFINE_HASH_MAP_GET(n, a, kt, vt)                                      \
    int n ## _get(PT_ENABLED vt *out, a *map, kt key) {                        \
        uint8 hash_out[HASH_LEN];                                              \
        siphash(&key, sizeof(kt), map->k, hash_out, HASH_LEN);                 \
        uint32 bucket = U8TO64_BE(hash_out) % map->size;                       \
        uint32 offset = 0;                                                     \
        while (offset < map->size &&                                           \
               IS_OCCUPIED(*map, OFFSET_BUCKET(bucket, offset))) {             \
            PT_COUNTER_INC(0, 1);                                              \
            if (map->entry_arr[OFFSET_BUCKET(bucket, offset)].key == key) {    \
                *out = map->entry_arr[OFFSET_BUCKET(bucket, offset)].val;      \
                return 0;                                                      \
            }                                                                  \
            offset += 1;                                                       \
        }                                                                      \
        PT_COUNTER_INC(1, 1);                                                  \
        return 1;                                                              \
    }

#define DEFINE_HASH_MAP_PRINT(n, a, kf, vf)                                    \
    void n ## _print(a *map) {                                                 \
        int i;                                                                 \
        printf("{\n");                                                         \
    	for (i = 0; i < map->size; ++i) {                                      \
            if (IS_OCCUPIED(*map, i)) {                                        \
                printf(                                                        \
                        "  [%d] " #kf ": " #vf "\n",                           \
                        i,                                                     \
                        map->entry_arr[i].key,                                 \
                        map->entry_arr[i].val                                  \
                );                                                             \
            } else {                                                           \
                printf("  [%d]\n", i);                                         \
            }                                                                  \
    	}                                                                      \
        printf("}\n");                                                         \
        for (i = 0; i < OCC_SIZE(map->size); ++i) {                            \
            printf("%08b\n", map->occ_arr[i]);                                 \
        }                                                                      \
    }

DEFINE_HASH_MAP_CREATE(hash_map_cc, HashMapCC);
DEFINE_HASH_MAP_CREATE(hash_map_ci, HashMapCI);

DEFINE_HASH_MAP_DESTROY(hash_map_cc, HashMapCC);
DEFINE_HASH_MAP_DESTROY(hash_map_ci, HashMapCI);

DEFINE_HASH_MAP_INSERT(hash_map_cc, HashMapCC, char, char);
DEFINE_HASH_MAP_INSERT(hash_map_ci, HashMapCI, char, int);

DEFINE_HASH_MAP_DELETE(hash_map_cc, HashMapCC, char);
DEFINE_HASH_MAP_DELETE(hash_map_ci, HashMapCI, char);

DEFINE_HASH_MAP_GET(hash_map_cc, HashMapCC, char, char);
DEFINE_HASH_MAP_GET(hash_map_ci, HashMapCI, char, int);

DEFINE_HASH_MAP_PRINT(hash_map_cc, HashMapCC, %c, %c);
DEFINE_HASH_MAP_PRINT(hash_map_ci, HashMapCI, %c, %d);
