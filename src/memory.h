#ifndef _HG_MEMORY_H_
#define _HG_MEMORY_H_
#include "common.h"

/* flexible struct:
 * struct flex {
 *     size_t len;
 *     type arr[];
 * };
 */
#define flexible_size_(st, type, len)     \
    ({                                    \
        size_t _len = len;                \
        sizeof(st) + sizeof(type) * _len; \
    })
#define flexible_alloc_(st, type, len)                \
    ({                                                \
        size_t _size = flexible_size_(st, type, len); \
        (st*)hg_realloc(NULL, 0, _size);              \
    })
#define flexible_realloc_(ptr, st, type, old_len, new_len)    \
    ({                                                        \
        size_t _old_size = flexible_size_(st, type, old_len); \
        size_t _new_size = flexible_size_(st, type, new_len); \
        (st*)hg_realloc(ptr, _old_size, _new_size);           \
    })
#define flexible_free_(ptr, st, type, len)            \
    do {                                              \
        size_t _size = flexible_size_(st, type, len); \
        hg_realloc(ptr, _size, 0);                    \
    } while (0)

/* array:
 */
#define array_alloc_(type, len)                          \
    ({                                                   \
        size_t _len = (len);                             \
        (type*)hg_realloc(NULL, 0, _len * sizeof(type)); \
    })
#define array_realloc_(ptr, type, old_len, new_len)                           \
    ({                                                                        \
        size_t _old_len = (old_len);                                          \
        size_t _len     = (len);                                              \
        (type*)hg_realloc(ptr, _old_len * sizeof(type), _len * sizeof(type)); \
    })
#define array_free_(ptr, type, len)              \
    do {                                         \
        size_t _len = (len);                     \
        hg_realloc(ptr, sizeof(type) * _len, 0); \
    } while (0)

#define hg_alloc_(type) ((type*)hg_realloc(NULL, 0, sizeof(type)))
#define hg_free_(type, ptr)               \
    do {                                  \
        hg_realloc(ptr, sizeof(type), 0); \
    } while (0)

void* hg_realloc(void* ptr, size_t old_size, size_t new_size);

#endif // _HG_MEMORY_H_
