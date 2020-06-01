#ifndef _HG_HASH_H_
#define _HG_HASH_H_
#include "value.h"

#define HASH_INITIAL_SIZE (3u)
#define HASH_LOAD_FACTOR  (0.75)
// the maximum number of tombstones is capacity*HASH_TOMB_FACTOR
#define HASH_TOMB_FACTOR (0.3)

//> hash_set
//NOTE: if there are tombstones in the set, then `() in set` is true.
//      DO NOT use hash_set to store nil.
struct hash_set {
    size_t capacity;
    size_t len; // valid and tombstone items
    size_t tomb_cnt;
    // empty if item.type == HG_VALUE_UNDEF
    // tombstone if item.type == HG_VALUE_NIL
    struct hg_value* items;
};

void hash_set_init_with_capacity(struct hash_set* set, size_t capacity);
static _force_inline_ void hash_set_init(struct hash_set* set) {
    hash_set_init_with_capacity(set, HASH_INITIAL_SIZE);
}
void hash_set_free(struct hash_set* set);
int hash_set_write(struct hash_set* set, FILE* fp);

void hash_set_put(struct hash_set* set, struct hg_value item);
bool hash_set_contain(struct hash_set* set, struct hg_value item);
struct hg_value hash_set_remove(struct hash_set* set, struct hg_value item);

// used for string pool. When the first time to insert a string, the pool
// needs `strcmp` to determine if two strings are the same.
typedef bool (*value_equal_func)(struct hg_value, struct hg_value);
bool hash_set_contain_with_equal(struct hash_set* set, struct hg_value item,
                                 value_equal_func func);
//< hash_set

//> hash_entry
struct hash_map_entry {
    // tombstone if key.type == HG_VALUE_UNDEF and value.type == HG_VALUE_NIL
    // empty entry if key.type == HG_VALUE_UNDEF and value.type == HG_VALUE_UNDEF
    struct hg_value key;
    struct hg_value value;
};
//< hash_entry

//> hash_map
struct hash_map {
    size_t capacity;
    size_t len;      // the number of valid entries and tombstones
    size_t tomb_cnt; // the number of tombstones
    struct hash_map_entry* entries;
};

void hash_map_init_with_capacity(struct hash_map* map, size_t capacity);
static _force_inline_ void hash_map_init(struct hash_map* map) {
    hash_map_init_with_capacity(map, HASH_INITIAL_SIZE);
}
void hash_map_free(struct hash_map* map);
int hash_map_write(struct hash_map* map, FILE* fp);
void hash_map_put(struct hash_map* map, struct hg_value key,
                  struct hg_value val);
bool hash_map_contain(struct hash_map* map, struct hg_value key);
struct hg_value hash_map_get(struct hash_map* map, struct hg_value key);
struct hg_value hash_map_remove(struct hash_map* map, struct hg_value key);
//< hash_map
#endif // _HG_HASH_H_
