#include "hash.h"
#include "memory.h"

//> hash_map_entry
static inline bool is_tombstone(struct hash_map_entry* entry) {
    return VAL_IS_UNDEF(entry->key) && VAL_IS_NIL(entry->value);
}
static inline bool is_empty(struct hash_map_entry* entry) {
    return VAL_IS_UNDEF(entry->key) && VAL_IS_UNDEF(entry->value);
}
static inline bool is_valid(struct hash_map_entry* entry) {
    return !VAL_IS_UNDEF(entry->key);
}
//< hash_map_entry

//> hash_map
int hash_map_write(struct hash_map* map, FILE* fp) {
    fprintf(fp, "{");
    for (size_t i = 0, flag = 0; i < map->capacity; i++) {
        if (flag) {
            fprintf(fp, ", ");
            flag = 1;
        }
        struct hash_map_entry* entry = &map->entries[i];
        if (is_valid(entry)) {
            hg_value_write(entry->key, fp);
            fprintf(fp, ": ");
            hg_value_write(entry->value, fp);
        }
    }
    fprintf(fp, "}");
    return 1;
}

static inline void hash_map_init_with_capacity(struct hash_map* map,
                                               size_t capacity) {
    map->len      = 0;
    map->tomb_cnt = 0;
    map->capacity = capacity;

    // initialize all value in entries to be HG_VALUE_UNDEF
    map->entries = array_alloc_init_(struct hash_map_entry, capacity, 0);
}

void hash_map_init(struct hash_map* map) {
    hash_map_init_with_capacity(map, HASH_INITIAL_SIZE);
}

void hash_map_free(struct hash_map* map) {
    // TODO: free object-value in entries? Or GC?
    array_free_(map->entries, struct hash_map_entry, map->capacity);
}

static void hash_map_resize(struct hash_map* map, size_t new_capacity) {
    struct hash_map new_map;
    hash_map_init_with_capacity(&new_map, new_capacity);

    for (size_t i = 0; i < map->capacity; i++) {
        struct hash_map_entry* entry = &map->entries[i];
        if (is_valid(entry)) {
            hash_map_put(&new_map, *entry);
        }
    }
    hash_map_free(map);

    *map = new_map;
}

inline static void hash_map_adjust(struct hash_map* map) {
    if (map->capacity * HASH_TOMB_FACTOR < map->tomb_cnt) {
        hash_map_resize(map, map->capacity);
    } else if (map->capacity * HASH_LOAD_FACTOR < map->len) {
        hash_map_resize(map, map->capacity * 2);
    }
}

static struct hash_map_entry* hash_map_find(struct hash_map* map,
                                            struct hg_value key) {
    assert(!VAL_IS_UNDEF(key));
    uint32_t hash = hg_value_hash(key) % map->capacity;

    // the first tombstone in the probe list
    struct hash_map_entry* tombstone = NULL;
    while (1) {
        struct hash_map_entry* entry = &map->entries[hash];

        if (is_tombstone(entry)) {
            if (tombstone != NULL)
                tombstone = entry;
        } else if (is_empty(entry)) {
            return tombstone != NULL ? tombstone : entry;
        } else if (hg_value_equal(entry->key, key)) {
            // key is not UNDEF, so there is no need to check if entry->key is valid
            return entry;
        }
        hash = (hash + 1) % map->capacity;
    }
    unreachable_();
    //return NULL; // make gcc happy
}

void hash_map_put(struct hash_map* map, struct hash_map_entry entry) {
    hash_map_adjust(map);

    struct hash_map_entry* ptr = hash_map_find(map, entry.key);

    if (is_empty(ptr))
        map->len++;
    if (is_tombstone(ptr))
        map->tomb_cnt--;

    *ptr = entry;
}

bool hash_map_contain(struct hash_map* map, struct hg_value key) {
    return is_valid(hash_map_find(map, key));
}

struct hg_value hash_map_get(struct hash_map* map, struct hg_value key) {
    struct hash_map_entry* ptr = hash_map_find(map, key);

    // return UNDEF if !is_valid
    return is_valid(ptr) ? ptr->value : VAL_UNDEF();
}

struct hg_value hash_map_remove(struct hash_map* map, struct hg_value key) {
    struct hash_map_entry* ptr = hash_map_find(map, key);

    struct hg_value t = VAL_UNDEF();

    if (is_valid(ptr)) {
        t = ptr->value;
        // make tombstone
        map->tomb_cnt += 1;
        ptr->key   = VAL_UNDEF();
        ptr->value = VAL_NIL();
    }

    return t; // return UNDEF if !is_valid
}
//< hash_map
