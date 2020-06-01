#include "hash.h"
#include "value.h"
#include "memory.h"
#include "string.h"

int main() {
    const char* arr[] = {
        "yangtau",
        "tests for set",
        "tests for hash",
        "yangli",
        "xjj",
        "tests for hash set",
        "hello",
        "yangtau",
        "world",
        "yangli",
        "hello world",
        "tests for hash map",
        "yangtau",
        NULL,
    };
    for (int i = 0; arr[i]; i++) {
        printf("%d %s\n", i, arr[i]);
    }
    printf("\n");
    {
        // hash map
        printf("## tests for hash map\n");
        struct hash_map map;
        hash_map_init(&map);
        for (int i = 0; arr[i]; i++) {
            const char* s = arr[i];
            hash_map_put(&map, VAL_OBJ(hg_string_copy(s, strlen(s))),
                         VAL_INT(i));
            hash_map_write(&map, stdout);
            printf("\n");
        }
        hash_map_free(&map);
    }
    printf("\n");
    {
        // hash set
        printf("## tests for hash set\n");
        struct hash_set set;
        hash_set_init(&set);
        for (int i = 0; arr[i]; i++) {
            const char* s = arr[i];
            hash_set_put(&set, VAL_OBJ(hg_string_copy(s, strlen(s))));
            hash_set_write(&set, stdout);
            printf("\n");
        }
        hash_set_free(&set);
    }

    assert(hg_memory_usage() == 0u);
    return 0;
}
