#include "hashset.h"
#include "memory.h"
#include <string.h>

void hg_hashset_init(struct hg_hashset* set, size_t capacity, hg_hash_hashfunc hash, hg_hash_eqfunc eq) {
    set->capacity = capacity;
    set->len      = 0;
    set->tomb_cnt = 0;
    set->hash     = hash;
    set->eq       = eq;
    set->buckets  = hg_alloc(capacity * sizeof(struct hg_hashset_bucket));

	// TODO: use calloc
    memset(set->buckets, 0, capacity * sizeof(struct hg_hashset_bucket));
}

inline void hg_hashset_destroy(struct hg_hashset* set) {
    hg_free(set->buckets);
}

// Returns the ptr of the bucket containing the item, or the ptr of the first empty or tombstone bucket if the item
// is not present.
static struct hg_hashset_bucket* hashset_find(struct hg_hashset* set, void* item) {
    uint32_t hash = set->hash(item) % set->capacity;

    struct hg_hashset_bucket* tombstone = NULL;
    for (size_t i = 0; i < set->capacity; i++) {
        struct hg_hashset_bucket* bucket = &set->buckets[hash];
        switch (bucket->state) {
        case HG_HASHSET_BUCKET_EMPTY:
            // If we found an empty bucket, we can stop searching, because we know the item is not in the set.
            return tombstone ? tombstone : bucket;
        case HG_HASHSET_BUCKET_OCCUPIED:
            if (set->eq(bucket->item, item))
                return bucket;
            break;
        case HG_HASHSET_BUCKET_TOMBSTONE:
            if (!tombstone)
                tombstone = bucket;
            break;
        }

        hash = (hash + 1) % set->capacity;
    }
    unreachable_();
}

static void hashset_resize(struct hg_hashset* set, size_t capacity) {
    struct hg_hashset new_set;
    hg_hashset_init(&new_set, capacity, set->hash, set->eq);
    for (size_t i = 0; i < set->capacity; i++) {
        struct hg_hashset_bucket* ptr = &set->buckets[i];
        if (ptr->state == HG_HASHSET_BUCKET_OCCUPIED)
            hg_hashset_put(&new_set, ptr->item);
    }
    hg_hashset_destroy(set);
    *set = new_set;
}

static inline void hashset_adjust(struct hg_hashset* set) {
    if (set->capacity * HG_HASHSET_LOAD_FACTOR < set->len) {
        hashset_resize(set, set->capacity * 2);
    } else if (set->capacity * HG_HASHSET_TOMB_FACTOR < set->tomb_cnt) {
        hashset_resize(set, set->capacity);
    }
}

void hg_hashset_put(struct hg_hashset* set, void* item) {
    hashset_adjust(set);

    struct hg_hashset_bucket* bucket = hashset_find(set, item);

    switch (bucket->state) {
    case HG_HASHSET_BUCKET_EMPTY:
        bucket->state = HG_HASHSET_BUCKET_OCCUPIED;
        bucket->item  = item;
        set->len++;
        break;
    case HG_HASHSET_BUCKET_OCCUPIED:
        bucket->item = item;
        break;
    case HG_HASHSET_BUCKET_TOMBSTONE:
        bucket->state = HG_HASHSET_BUCKET_OCCUPIED;
        bucket->item  = item;
        set->tomb_cnt--;
        break;
    }
}

void* hg_hashset_get(struct hg_hashset* set, void* item) {
    struct hg_hashset_bucket* bucket = hashset_find(set, item);
    return bucket->state == HG_HASHSET_BUCKET_OCCUPIED ? bucket->item : NULL;
}

bool hashset_contains(struct hg_hashset* set, void* item) {
    return hashset_find(set, item)->state == HG_HASHSET_BUCKET_OCCUPIED;
}

void* hg_hashset_remove(struct hg_hashset* set, void* item) {
    struct hg_hashset_bucket* bucket = hashset_find(set, item);
    if (bucket->state != HG_HASHSET_BUCKET_OCCUPIED)
        return NULL;

    bucket->state = HG_HASHSET_BUCKET_TOMBSTONE;
    set->tomb_cnt++;
    return bucket->item;
}

void hg_hashset_clear(struct hg_hashset* set) {
    for (size_t i = 0; i < set->capacity; i++) {
        set->buckets[i].state = HG_HASHSET_BUCKET_EMPTY;
    }

    set->len      = 0;
    set->tomb_cnt = 0;
}

void hg_hashset_foreach(struct hg_hashset* set, void (*fn)(void*)) {
    for (size_t i = 0; i < set->capacity; i++) {
        struct hg_hashset_bucket* bucket = &set->buckets[i];
        if (bucket->state == HG_HASHSET_BUCKET_OCCUPIED)
            fn(bucket->item);
    }
}
