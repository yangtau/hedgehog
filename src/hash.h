#ifndef _HG_HASH_H_
#define _HG_HASH_H_
#include "value.h"

#define HASH_INITIAL_SIZE (4u)
#define HASH_LOAD_FACTOR  (0.75)
// the maximum number of tombstones is capacity*HASH_TOMB_FACTOR
#define HASH_TOMB_FACTOR (0.3)

//> hash_set
struct hash_set {
    size_t capacity;
    size_t len;
    struct hg_value *items;
};

void hash_set_init(struct hash_set* set);
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

void hash_map_new(struct hash_map* map);
void hash_map_free(struct hash_map* map);
int hash_map_write(struct hash_map* map, FILE* fp);
void hash_map_put(struct hash_map* map, struct hash_map_entry entry);
bool hash_map_contain(struct hash_map* map, struct hg_value key);
struct hg_value hash_map_get(struct hash_map* map, struct hg_value key);
struct hg_value hash_map_remove(struct hash_map* map, struct hg_value key);
//< hash_map
#endif // _HG_HASH_H_
