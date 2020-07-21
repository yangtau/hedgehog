#ifndef _HG_MEMORY_H_
#define _HG_MEMORY_H_
#include "common.h"

//> flexible
/* flexible struct:
 * struct flex {
 *     size_t len;
 *     type arr[];
 * };
 */
#define flexible_size_(st, type, capacity)     \
    ({                                         \
        size_t _capacity = (capacity);         \
        sizeof(st) + sizeof(type) * _capacity; \
    })
#define flexible_alloc_(st, type, capacity)                \
    ({                                                     \
        size_t _size = flexible_size_(st, type, capacity); \
        (st*)hg_realloc(NULL, 0, _size);                   \
    })
#define flexible_alloc_init_(st, type, capacity, init)     \
    ({                                                     \
        size_t _size = flexible_size_(st, type, capacity); \
        void* _ptr   = hg_realloc(NULL, 0, _size);         \
        memset(_ptr, init, _size);                         \
        (st*)_ptr;                                         \
    })
#define flexible_realloc_(ptr, st, type, old_capacity, new_capacity) \
    ({                                                               \
        size_t _old_size = flexible_size_(st, type, old_capacity);   \
        size_t _new_size = flexible_size_(st, type, new_capacity);   \
        (st*)hg_realloc(ptr, _old_size, _new_size);                  \
    })
#define flexible_free_(ptr, st, type, capacity)            \
    do {                                                   \
        size_t _size = flexible_size_(st, type, capacity); \
        hg_realloc(ptr, _size, 0);                         \
    } while (0)
//< flexible

//> array
#define array_alloc_(type, capacity)                          \
    ({                                                        \
        size_t _capacity = (capacity);                        \
        (type*)hg_realloc(NULL, 0, _capacity * sizeof(type)); \
    })
#define array_alloc_init_(type, capacity, init)    \
    ({                                             \
        size_t _size = (capacity) * sizeof(type);  \
        void* ptr    = hg_realloc(NULL, 0, _size); \
        memset(ptr, init, _size);                  \
        (type*)ptr;                                \
    })
#define array_grow_(capacity, len)      \
    ({                                  \
        size_t _cap = (capacity);       \
        size_t _len = (len);            \
        _len >= _cap ? _cap * 2 : _cap; \
    })
#define array_shrink_(capacity, len)        \
    ({                                      \
        size_t _cap = (capacity);           \
        size_t _len = (len);                \
        _len <= _cap / 4 ? _cap / 2 : _cap; \
    })
#define array_realloc_(ptr, type, old_capacity, new_capacity) \
    ({                                                        \
        size_t _old_capacity = (old_capacity);                \
        size_t _capacity     = (new_capacity);                \
        (type*)hg_realloc(ptr, _old_capacity * sizeof(type),  \
                          _capacity * sizeof(type));          \
    })
#define array_free_(ptr, type, capacity)              \
    do {                                              \
        size_t _capacity = (capacity);                \
        hg_realloc(ptr, sizeof(type) * _capacity, 0); \
    } while (0)
//< array

//> hg_alloc
#define hg_alloc_(type) ((type*)hg_realloc(NULL, 0, sizeof(type)))
#define hg_free_(type, ptr)               \
    do {                                  \
        hg_realloc(ptr, sizeof(type), 0); \
    } while (0)

void* hg_realloc(void* ptr, size_t old_size, size_t new_size);
//< hg_alloc

size_t hg_memory_usage();
#endif // _HG_MEMORY_H_
