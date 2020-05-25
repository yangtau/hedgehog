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
int hash_map_write(struct hg_object* _map, FILE* fp) {
    struct hash_map* map = (struct hash_map*)_map;
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

static inline struct hash_map* hash_map_new_with_capacity(size_t capacity) {
    // initialize all value in entries to be HG_VALUE_UNDEF
    struct hash_map* map = flexible_alloc_init_(
        struct hash_map, struct hash_map_entry, capacity, 0);

    map->len      = 0;
    map->tomb_cnt = 0;
    map->capacity = capacity;
    return map;
}

struct hash_map* hash_map_new() {
    return hash_map_new_with_capacity(HASH_INITIAL_SIZE);
}

void hash_map_free(struct hash_map* map) {
    // TODO: free object-value in entries? Or GC?
    flexible_free_(map, struct hash_map, struct hash_map_entry, map->capacity);
}

static struct hash_map* hash_map_resize(struct hash_map* map,
                                        size_t new_capacity) {
    struct hash_map* new_map = hash_map_new_with_capacity(new_capacity);

    for (size_t i = 0; i < map->capacity; i++) {
        struct hash_map_entry* entry = &map->entries[i];
        if (is_valid(entry)) {
            hash_map_put(&new_map, *entry);
        }
    }

    hash_map_free(map);
    return new_map;
}

inline static struct hash_map* hash_map_adjust(struct hash_map* map) {
    if (map->capacity * HASH_TOMB_FACTOR < map->tomb_cnt) {
        return hash_map_resize(map, map->capacity);
    } else if (map->capacity * HASH_LOAD_FACTOR < map->len) {
        return hash_map_resize(map, map->capacity * 2);
    }
    return map;
}

static struct hash_map_entry* hash_map_find(struct hash_map* map,
                                            struct hg_value key) {
    assert(!VAL_IS_UNDEF(key));
    uint32_t hash = hg_value_hash(key) % map->capacity;
    while (1) {
        struct hash_map_entry* entry = &map->entries[hash];
        if (hg_value_equal(entry->key, key)
            // key is not UNDEF, so there is no need to check if entry->key is valid
            || is_empty(entry)) {
            return entry;
        }
        hash = (hash + 1) % map->capacity;
    }
    unreachable_();
    //return NULL; // make gcc happy
}

void hash_map_put(struct hash_map** map, struct hash_map_entry entry) {
    *map = hash_map_adjust(*map);

    struct hash_map_entry* ptr = hash_map_find(*map, entry.key);
    assert(!is_tombstone(ptr));
    if (is_empty(ptr))
        (*map)->len++;
    *ptr = entry;
}

bool hash_map_contain(struct hash_map* map, struct hg_value key) {
    struct hash_map_entry* ptr = hash_map_find(map, key);
    assert(!is_tombstone(ptr));
    return is_valid(ptr);
}

struct hg_value hash_map_get(struct hash_map* map, struct hg_value key) {
    struct hash_map_entry* ptr = hash_map_find(map, key);
    assert(!is_tombstone(ptr));
    return ptr->value; // return UNDEF if is_empty
}

struct hg_value hash_map_remove(struct hash_map* map, struct hg_value key) {
    struct hash_map_entry* ptr = hash_map_find(map, key);
    assert(!is_tombstone(ptr));
    struct hg_value value = ptr->value;

    // make tombstone
    map->tomb_cnt += 1;
    ptr->value = VAL_NIL();
    ptr->key   = VAL_UNDEF();

    return value; // return UNDEF if is_empty
}
//< hash_map
