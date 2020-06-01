#include "hash.h"
#include "memory.h"
#include "value.h"

//> hash_set
static inline bool item_is_empty(struct hg_value* val) {
    return VAL_IS_UNDEF(*val);
}
static inline bool item_is_tombstone(struct hg_value* val) {
    return VAL_IS_NIL(*val);
}
static inline bool item_is_valid(struct hg_value* val) {
    return !VAL_IS_UNDEF(*val) && !VAL_IS_NIL(*val);
}
int hash_set_write(struct hash_set* set, FILE* fp) {
    fprintf(fp, "{");
    bool flag = false;
    for (size_t i = 0; i < set->capacity; i++) {
        struct hg_value* item = &set->items[i];
        if (item_is_valid(item)) {
            if (flag) {
                fprintf(fp, ", ");
            }
            flag = true;
            hg_value_write(*item, fp);
        }
    }
    fprintf(fp, "}");
    return 1;
}

void hash_set_init_with_capacity(struct hash_set* set, size_t capacity) {
    set->len      = 0;
    set->tomb_cnt = 0;
    set->capacity = capacity;
    set->items    = array_alloc_init_(struct hg_value, capacity, 0);
}

void hash_set_free(struct hash_set* set) {
    array_free_(set->items, struct hg_value, set->capacity);
    set->len      = 0;
    set->capacity = 0;
    set->tomb_cnt = 0;
    set->items    = NULL;
}

static struct hg_value* hash_set_find(struct hash_set* set,
                                      struct hg_value item,
                                      value_equal_func func) {
    assert(!VAL_IS_UNDEF(item));
    uint32_t hash = hg_value_hash(item) % set->capacity;

    struct hg_value* tombstone = NULL;
    while (1) {
        struct hg_value* ptr = &set->items[hash];
        if (item_is_tombstone(ptr)) {
            tombstone = ptr;
        } else if (item_is_empty(ptr)) {
            return tombstone != NULL ? tombstone : ptr;
        } else if (func(*ptr, item)) {
            return ptr;
        }
        hash = (hash + 1) % set->capacity;
    }
    unreachable_();
}

static void hash_set_resize(struct hash_set* set, size_t capacity) {
    struct hash_set new_set;
    hash_set_init_with_capacity(&new_set, capacity);
    for (size_t i = 0; i < set->capacity; i++) {
        struct hg_value* item = &set->items[i];
        if (item_is_valid(item)) {
            hash_set_put(&new_set, *item);
        }
    }
    hash_set_free(set);
    *set = new_set;
}

static inline void hash_set_adjust(struct hash_set* set) {
    if (set->capacity * HASH_LOAD_FACTOR < set->len) {
        hash_set_resize(set, set->capacity * 2);
    } else if (set->capacity * HASH_TOMB_FACTOR < set->tomb_cnt) {
        hash_set_resize(set, set->capacity);
    }
}

void hash_set_put(struct hash_set* set, struct hg_value item) {
    struct hg_value* ptr = hash_set_find(set, item, hg_value_equal);

    if (item_is_empty(ptr))
        set->len++;
    else if (item_is_tombstone(ptr))
        set->tomb_cnt--;
    *ptr = item;

    hash_set_adjust(set);
}

bool hash_set_contain(struct hash_set* set, struct hg_value item) {
    return item_is_valid(hash_set_find(set, item, hg_value_equal));
}

bool hash_set_contain_with_equal(struct hash_set* set, struct hg_value item,
                                 value_equal_func func) {
    return item_is_valid(hash_set_find(set, item, func));
}

struct hg_value hash_set_remove(struct hash_set* set, struct hg_value item) {
    struct hg_value* ptr = hash_set_find(set, item, hg_value_equal);
    struct hg_value t    = VAL_UNDEF();

    if (item_is_valid(ptr)) {
        t = *ptr;
        set->tomb_cnt++;
        *ptr = VAL_NIL(); // make a tombstone
    }
    return t;
}
//< hash_set

//> hash_map_entry
static inline bool entry_is_tombstone(struct hash_map_entry* entry) {
    return VAL_IS_UNDEF(entry->key) && VAL_IS_NIL(entry->value);
}
static inline bool entry_is_empty(struct hash_map_entry* entry) {
    return VAL_IS_UNDEF(entry->key) && VAL_IS_UNDEF(entry->value);
}
static inline bool entry_is_valid(struct hash_map_entry* entry) {
    return !VAL_IS_UNDEF(entry->key);
}
//< hash_map_entry

//> hash_map
int hash_map_write(struct hash_map* map, FILE* fp) {
    fprintf(fp, "{");
    bool flag = false;
    for (size_t i = 0; i < map->capacity; i++) {
        struct hash_map_entry* entry = &map->entries[i];
        if (entry_is_valid(entry)) {
            if (flag) {
                fprintf(fp, ", ");
            }
            flag = true;

            hg_value_write(entry->key, fp);
            fprintf(fp, ": ");
            hg_value_write(entry->value, fp);
        }
    }
    fprintf(fp, "}");
    return 1;
}

void hash_map_init_with_capacity(struct hash_map* map, size_t capacity) {
    map->len      = 0;
    map->tomb_cnt = 0;
    map->capacity = capacity;

    // initialize all value in entries to be HG_VALUE_UNDEF
    map->entries = array_alloc_init_(struct hash_map_entry, capacity, 0);
}

void hash_map_free(struct hash_map* map) {
    array_free_(map->entries, struct hash_map_entry, map->capacity);
    map->capacity = 0;
    map->len      = 0;
    map->entries  = NULL;
    map->tomb_cnt = 0;
}

static void hash_map_resize(struct hash_map* map, size_t new_capacity) {
    struct hash_map new_map;
    hash_map_init_with_capacity(&new_map, new_capacity);

    for (size_t i = 0; i < map->capacity; i++) {
        struct hash_map_entry* entry = &map->entries[i];
        if (entry_is_valid(entry)) {
            hash_map_put(&new_map, entry->key, entry->value);
        }
    }
    hash_map_free(map);

    *map = new_map;
}

inline static void hash_map_adjust(struct hash_map* map) {
    if (map->capacity * HASH_LOAD_FACTOR < map->len) {
        hash_map_resize(map, map->capacity * 2);
    } else if (map->capacity * HASH_TOMB_FACTOR < map->tomb_cnt) {
        hash_map_resize(map, map->capacity);
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

        if (entry_is_tombstone(entry)) {
            tombstone = entry;
        } else if (entry_is_empty(entry)) {
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

void hash_map_put(struct hash_map* map, struct hg_value key,
                  struct hg_value val) {
    struct hash_map_entry entry = {key, val};
    struct hash_map_entry* ptr  = hash_map_find(map, entry.key);

    if (entry_is_empty(ptr))
        map->len++;
    else if (entry_is_tombstone(ptr))
        map->tomb_cnt--;

    *ptr = entry;

    hash_map_adjust(map);
}

bool hash_map_contain(struct hash_map* map, struct hg_value key) {
    return entry_is_valid(hash_map_find(map, key));
}

struct hg_value hash_map_get(struct hash_map* map, struct hg_value key) {
    struct hash_map_entry* ptr = hash_map_find(map, key);

    // return UNDEF if !is_valid
    return entry_is_valid(ptr) ? ptr->value : VAL_UNDEF();
}

struct hg_value hash_map_remove(struct hash_map* map, struct hg_value key) {
    struct hash_map_entry* ptr = hash_map_find(map, key);

    struct hg_value t = VAL_UNDEF();

    if (entry_is_valid(ptr)) {
        t = ptr->value;
        // make tombstone
        map->tomb_cnt += 1;
        ptr->key   = VAL_UNDEF();
        ptr->value = VAL_NIL();
    }

    return t; // return UNDEF if !is_valid
}
//< hash_map
