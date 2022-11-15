#include "test.h"
#include "../hashset.h"

uint32_t hash_int(const void* item) {
    uint32_t x = (uint32_t)(uint64_t)(item);

    x = ((x >> 16) ^ x) * 0x119de1f3;
    // x = ((x >> 16) ^ x) * 0x119de1f3;
    // x = (x >> 16) ^ x;
    return x;
}

bool eq_int(const void* a, const void* b) {
    return a == b;
}

void print_int(void* item) {
    println((uint64_t)(item));
}

void test_hashset() {
    struct hg_hashset set;
    hg_hashset_init(&set, 2, hash_int, eq_int);

    const int N = 192 * 1024;

    // put and contains
    duration_t start = now();
    for (int i = 0; i < N; i++) {
        hg_hashset_put(&set, (void*)(uint64_t)i);
    }
    duration_t elapsed = since(start);
    test_println("hashset put: %d items in %s", N, duration2str(elapsed, MACROSEC));
    test_println("hashset put %lld ns/op", (elapsed / MACROSEC * 1000) / N);
    test_println("hashset load factor: %f", (float)set.len / set.capacity);
    test_println("hashset cap: %zu", set.capacity);

    test_assert(hg_hashset_len(&set) == N);

    // get
    start = now();
    for (int i = 0; i < N; i++) {
        test_assert(hg_hashset_get(&set, (void*)(uint64_t)i) == (void*)(uint64_t)i);
    }
    elapsed = since(start);
    test_println("hashset get %lld ns/op", (elapsed / MACROSEC * 1000) / N);

    // contains
    start = now();
    for (int i = 0; i < N; i++) {
        test_assert(hg_hashset_contains(&set, (void*)(uint64_t)i));
    }
    elapsed = since(start);
    test_println("hashset contains %lld ns/op", (elapsed / MACROSEC * 1000) / N);

    // remove
    start = now();
    for (int i = 0; i < N; i++) {
        test_assert(hg_hashset_remove(&set, (void*)(uint64_t)i) == (void*)(uint64_t)i);
    }
    elapsed = since(start);
    test_println("hashset remove %lld ns/op", (elapsed / MACROSEC * 1000) / N);

    test_assert(hg_hashset_len(&set) == 0);

    // put again
    for (int i = 0; i < N; i++) {
        test_assert(!hg_hashset_contains(&set, (void*)(uint64_t)i));
        hg_hashset_put(&set, (void*)(uint64_t)i);
        test_assert(hg_hashset_contains(&set, (void*)(uint64_t)i));
    }
    test_assert(hg_hashset_len(&set) == N);

    // remove even numbers
    for (int i = 0; i < N; i += 2) {
        test_assert(hg_hashset_remove(&set, (void*)(uint64_t)i) == (void*)(uint64_t)i);
        test_assert(!hg_hashset_contains(&set, (void*)(uint64_t)i));
    }
    test_assert(hg_hashset_len(&set) == N / 2);

    // remove odd numbers
    for (int i = 1; i < N; i += 2) {
        test_assert(hg_hashset_remove(&set, (void*)(uint64_t)i) == (void*)(uint64_t)i);
        test_assert(!hg_hashset_contains(&set, (void*)(uint64_t)i));
    }
    test_assert(hg_hashset_len(&set) == 0);

    // clear
    hg_hashset_clear(&set);
    test_assert(hg_hashset_len(&set) == 0);

    // get again (expect NULL)
    for (int i = 0; i < N; i++) {
        test_assert(hg_hashset_get(&set, (void*)(uint64_t)i) == NULL);
    }
    test_assert(hg_hashset_len(&set) == 0);

    // destroy & init again
    hg_hashset_destroy(&set);
    hg_hashset_init(&set, 16, hash_int, eq_int);

    // put 0 ~ N-1
    for (int i = 0; i < N; i++) {
        hg_hashset_put(&set, (void*)(uint64_t)i);
    }
    test_assert(hg_hashset_len(&set) == N);

    // put N~2N-1, remove 0~N-1
    for (int i = 0; i < N; i++) {
        uint64_t x = i + N;
        test_assert(hg_hashset_remove(&set, (void*)(uint64_t)i) == (void*)(uint64_t)i);
        hg_hashset_put(&set, (void*)x);

        test_assert(hg_hashset_contains(&set, (void*)x));
        test_assert(!hg_hashset_contains(&set, (void*)(uint64_t)i));
    }
    test_assert(hg_hashset_len(&set) == N);

    // put 0~N-1, remove N~2N-1
    for (int i = 0; i < N; i++) {
        uint64_t x = i + N;
        hg_hashset_put(&set, (void*)(uint64_t)i);
        test_assert(hg_hashset_remove(&set, (void*)x) == (void*)x);

        test_assert(hg_hashset_contains(&set, (void*)(uint64_t)i));
        test_assert(!hg_hashset_contains(&set, (void*)x));
    }
    test_assert(hg_hashset_len(&set) == N);

    // put N~2N-1
    for (int i = 0; i < N; i++) {
        uint64_t x = i + N;
        hg_hashset_put(&set, (void*)x);
        test_assert(hg_hashset_contains(&set, (void*)x));
    }
    test_assert(hg_hashset_len(&set) == 2 * N);

    hg_hashset_destroy(&set);
}

void test_hashset_init() {
    struct hg_hashset set;
    int Ns[] = {1, 2, 3, 4, 5, 7, 11, 13, 17, 19};

    for (size_t i = 0; i < sizeof(Ns) / sizeof(int); i++) {
        int N = Ns[i];

        test_println("hashset init %d items", N);

        hg_hashset_init(&set, N, hash_int, eq_int);
        test_assert(hg_hashset_len(&set) == 0);

        // put N
        for (int i = 0; i < N; i++) {
            hg_hashset_put(&set, (void*)(uint64_t)i);
        }
        test_assert(hg_hashset_len(&set) == (size_t)N);

        // continas N?
        test_assert(!hg_hashset_contains(&set, (void*)(uint64_t)N));

        hg_hashset_destroy(&set);
    }
}

test_main(test_hashset, test_hashset_init)
