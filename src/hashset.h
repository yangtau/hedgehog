#ifndef HG_HASHSET_H_
#define HG_HASHSET_H_
#include "common.h"

typedef uint32_t (*hg_hash_hashfunc)(const void*);
typedef bool (*hg_hash_eqfunc)(const void*, const void*);

struct hg_hashset_bucket {
    void* item;
    enum {
        HG_HASHSET_BUCKET_EMPTY = 0, // default
        HG_HASHSET_BUCKET_OCCUPIED,
        HG_HASHSET_BUCKET_TOMBSTONE,
    } state;
};

struct hg_hashset {
    size_t capacity;
    size_t len; // the number of occupied buckets and tombstones
    size_t tomb_cnt;
    hg_hash_hashfunc hash;
    hg_hash_eqfunc eq;
    struct hg_hashset_bucket* buckets;
};

void hg_hashset_init(struct hg_hashset* set, size_t capacity, hg_hash_hashfunc hash, hg_hash_eqfunc eq);

void hg_hashset_destroy(struct hg_hashset* set);

// insert or update an item
void hg_hashset_put(struct hg_hashset* set, void* item);
// return NULL if not found.
// DO NOT check if the item exists by comparing the return value with NULL, use hg_hashset_contains instead.
// It also returns NULL if the user has inserted NULL.
void* hg_hashset_get(struct hg_hashset* set, void* item);
void* hg_hashset_remove(struct hg_hashset* set, void* item);
bool hg_hashset_contains(struct hg_hashset* set, void* item);
void hg_hashset_clear(struct hg_hashset* set);
void hg_hashset_foreach(struct hg_hashset* set, void (*fn)(void*));

static inline size_t hg_hashset_len(struct hg_hashset* set) {
    return set->len - set->tomb_cnt;
}

#endif // HG_HASHSET_H_
