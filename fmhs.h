#ifndef FMHS_H
#define FMHS_H

#include <stddef.h> // size_t, NULL


#ifndef FMNULLABLE // The value can be NULL
#define FMNULLABLE
#endif

#ifndef FMNONNULL // The value cannot be NULL
#define FMNONNULL
#endif

typedef int(fmhs_cmp_f)(FMNONNULL const void* item1ptr, FMNONNULL const void* item2ptr, size_t size_item);
typedef size_t(fmhs_hash_f)(FMNONNULL const void* itemptr, size_t size_item, size_t seed);

typedef struct {
    int* taked;
    unsigned char* items;
    size_t cap;
    size_t len;
    fmhs_cmp_f* eqf;
    fmhs_hash_f* hashf;
    size_t size_item;
    size_t iter_index;
    size_t seed;
} fmhs_t;

void fmhs_new(fmhs_t hs[static 1], size_t size_item, fmhs_cmp_f eqf, fmhs_hash_f hashf,
              size_t seed, size_t initial_cap);

void fmhs_delete(fmhs_t hs[static 1]);

int fmhs_isin(const fmhs_t hs[static 1], FMNONNULL const void* itemptr);

void fmhs_insert(fmhs_t hs[static 1], FMNONNULL const void* itemptr);
int fmhs_remove(fmhs_t hs[static 1], FMNONNULL const void* itemptr);

FMNULLABLE const void* fmhs_iter_begin(fmhs_t hs[static 1]);
FMNULLABLE const void* fmhs_iter_next(fmhs_t hs[static 1]);

#endif // FMHS_H

#ifdef FM_TEST
#define FMHS_IMPL
#endif

#ifdef FMHS_IMPL

#define FMHS_REALLOC_THRESHOLD 0.75
#define FMHS_GROWTH_RATE 2U

#ifdef FMNULLABLE
#undef FMNULLABLE
#endif
#define FMNULLABLE // The value can be NULL

#include <assert.h> // assert
#include <stdlib.h> // malloc, free
#include <string.h> // memset, memcpy

void fmhs_new(fmhs_t hs[static 1], size_t size_item, fmhs_cmp_f eqf, fmhs_hash_f hashf,
              size_t seed, size_t initial_cap) {

    hs->items = malloc(initial_cap * size_item);
    assert(hs->items && "fmhs_new_wcap: Could not allocate memory for items");

    hs->taked = malloc(initial_cap * sizeof(*hs->taked));
    assert(hs->taked && "fmhs_new_wcap: Could not allocate memory for taked");

    memset(hs->items, 0, initial_cap * size_item);
    memset(hs->taked, -1, initial_cap * sizeof(*hs->taked));
    hs->cap        = initial_cap;
    hs->eqf        = eqf;
    hs->hashf      = hashf;
    hs->size_item  = size_item;
    hs->iter_index = 0U;
    hs->len        = 0U;
    hs->seed       = seed;
}

int fmhs_isin(const fmhs_t hs[static 1], const void* itemptr) {
    size_t i     = hs->hashf(itemptr, hs->size_item, hs->seed) % hs->cap;
    size_t count = 0;
    while (count < hs->cap) {
        while (hs->taked[i] < 0) {
            ++i;
            ++count;
            if (count >= hs->cap)
                return 0;
            if (i >= hs->cap)
                i = 0;
        }
        if (hs->eqf(hs->items + (i * hs->size_item), itemptr, hs->size_item) == 0) {
            return 1;
        }
        ++i;
        ++count;
        if (count >= hs->cap)
            return 0;
        if (i >= hs->cap)
            i = 0;
    }
    return 0;
}

void fmhs_insert(fmhs_t hs[static 1], const void* itemptr) {
    assert(hs->cap > 0U &&
           "fmhs_insert: Capacity is zero, you need to first call fmhs_new");

    if ((double)hs->len / (double)hs->cap > FMHS_REALLOC_THRESHOLD) {
        fmhs_t new_hs = {0};
        fmhs_new(&new_hs, hs->size_item, hs->eqf, hs->hashf, hs->seed,
                 hs->cap * FMHS_GROWTH_RATE);
        for (const void* it = fmhs_iter_begin(hs); it != NULL;
             it             = fmhs_iter_next(hs)) {
            fmhs_insert(&new_hs, it);
        }
        fmhs_delete(hs);
        memcpy(hs, &new_hs, sizeof(*hs));
    }

    size_t i     = hs->hashf(itemptr, hs->size_item, hs->seed) % hs->cap;
    size_t count = 0U;
    while (!(hs->taked[i] < 0)) {
        if (hs->eqf(hs->items + (i * hs->size_item), itemptr, hs->size_item) == 0) {
            // Item already exists
            return;
        }
        assert(count < hs->cap && "fmhs_insert: Already iterates of all data");
        ++i;
        ++count;
        if (i >= hs->cap)
            i = 0U;
    }
    memcpy(hs->items + (i * hs->size_item), itemptr, hs->size_item);

    hs->taked[i] = 1;
    hs->len++;
    hs->iter_index = 0U; // We have a change, now any iter need to be restarted.
}

void fmhs_delete(fmhs_t hs[static 1]) {
    free(hs->items);
    free(hs->taked);
    memset(hs, 0, sizeof(*hs));
}

int fmhs_remove(fmhs_t hs[static 1], const void* itemptr) {
    size_t i     = hs->hashf(itemptr, hs->size_item, hs->seed) % hs->cap;
    size_t count = 0U;
    while (count < hs->cap) {
        if (hs->eqf(hs->items + (i * hs->size_item), itemptr, hs->size_item) == 0) {
            memset(hs->items + (i * hs->size_item), 0, hs->size_item);
            hs->taked[i] = -1;
            hs->len--;
            return 0;
        }
        ++i;
        ++count;
        if (!(i < hs->cap))
            i = 0U;
    }
    return -1;
}

const void* fmhs_iter_begin(fmhs_t hs[static 1]) {
    hs->iter_index = 0;
    return fmhs_iter_next(hs);
}

const void* fmhs_iter_next(fmhs_t hs[static 1]) {
    while (hs->iter_index < hs->cap) {
        if (!(hs->taked[hs->iter_index] < 0)) {
            hs->iter_index++;
            return hs->items + (hs->iter_index - 1U) * hs->size_item;
        }
        hs->iter_index++;
    }
    return NULL;
}

#endif // FMHS_IMPL

#ifdef FM_TEST
#include <stdio.h>

size_t hash(const void* itemptr, size_t itemsize, size_t seed) {
    (void)itemsize;
    (void)seed;
    if (*(const double*)itemptr < 0.0) {
        return (size_t) - *(const double*)itemptr;
    }
    return (size_t) * (const double*)itemptr;
}

int cmp(const void* item1ptr, const void* item2ptr, size_t itemsize) {
    (void)itemsize;
    assert(item1ptr);
    assert(item2ptr);
    return (int)(*(const double*)item1ptr - *(const double*)item2ptr);
}

int test_ok = 1;
#define xstr(a) str(a)
#define str(a) #a
#define TEST(func, ctx)                                                                \
    test_ok = 1;                                                                       \
    printf("TESTING:" xstr(func) " ");                                                 \
    fflush(stdout);                                                                    \
    ctx;                                                                               \
    test_ok ? printf(" OK!\n") : printf("\nTEST FAIL!\n");                             \
    test_ok ? (void)0 : exit(EXIT_FAILURE);                                            \
    (void)0

#define ASSERT(expr, msg)                                                              \
    do {                                                                               \
        if (!(expr)) {                                                                 \
            fprintf(stderr, "\n%s", msg);                                              \
            fflush(stderr);                                                            \
            test_ok = 0;                                                               \
        }                                                                              \
    } while (0)

int main(void) {
    TEST(fmhs_new, {
        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(double), cmp, hash, 33, 256U);

        ASSERT(hs.len == 0U, "fmhs.h: fmhs_new should set hs.len to zero");

        ASSERT(hs.cap == 256U, "fmhs.h: fmhs_new should set hs.cap to the capacity");

        ASSERT(hs.size_item == sizeof(double),
               "fmhs.h: fmhs_new should set hs.size_item to the sizeof the item");

        ASSERT(hs.eqf == cmp,
               "fmhs.h: fmhs_new should set hs.eqf to the comparition function");

        ASSERT(hs.hashf == hash,
               "fmhs.h: fmhs_new should set hs.hashf to the hash function");

        ASSERT(hs.taked != NULL,
               "fmhs.h: fmhs_new should set hs.taked to a non null pointer");

        ASSERT(hs.items != NULL,
               "fmhs.h: fmhs_new should set hs.items to a non null pointer");

        for (size_t i = 0U; i < hs.cap; ++i) {
            if (!(hs.taked[i] < 0)) {
                fprintf(stderr, "Error on hs.taked[%zu]\n", i);
                fflush(stderr);
                ASSERT((hs.taked[i] < 0),
                       "fmhs.h: fmhs_new should set all hs.taked to a negative value");
            }
        }

        for (size_t i = 0U; i < hs.cap * hs.size_item; ++i) {
            if (!(hs.items[i] == 0)) {
                fprintf(stderr, "Error on hs.items[%zu]\n", i);
                fflush(stderr);
                ASSERT((hs.items[i] == 0),
                       "fmhs.h: fmhs_new should set all bytes of hs.items to zero");
            }
        }
        fmhs_delete(&hs);
    });

    TEST(fmhs_insert, {
        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(double), cmp, hash, 33, 128U);
        fmhs_insert(&hs, &(double){2.0});
        ASSERT(hs.len == 1U, "fmhs.h: fmhs_insert should increase the hs.len. Item: 2");

        fmhs_insert(&hs, &(double){-2.0});
        ASSERT(hs.len == 2U,
               "fmhs.h: fmhs_insert should increase the hs.len. Item: -2");

        fmhs_insert(&hs, &(double){-3.0});
        ASSERT(hs.len == 3U,
               "fmhs.h: fmhs_insert should increase the hs.len. Item: -3");

        fmhs_insert(&hs, &(double){-7.0});
        ASSERT(hs.len == 4U,
               "fmhs.h: fmhs_insert should increase the hs.len. Item: -7");

        fmhs_insert(&hs, &(double){259.0});
        ASSERT(hs.len == 5U,
               "fmhs.h: fmhs_insert should increase the hs.len. Item: 259");
        fmhs_delete(&hs);
    });

    TEST(fmhs_isin, {
        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(double), cmp, hash, 33, 16U);
        fmhs_insert(&hs, &(double){2.0});
        fmhs_insert(&hs, &(double){-2.0});
        fmhs_insert(&hs, &(double){-3.0});
        fmhs_insert(&hs, &(double){-7.0});
        fmhs_insert(&hs, &(double){259.0});

        ASSERT(fmhs_isin(&hs, &(double){2.0}),
               "fmhs.h: fmhs_isin should return true for a inserted item: 2");
        ASSERT(fmhs_isin(&hs, &(double){-2.0}),
               "fmhs.h: fmhs_isin should return true for a inserted item: -2");
        ASSERT(fmhs_isin(&hs, &(double){-3.0}),
               "fmhs.h: fmhs_isin should return true for a inserted item: -3");
        ASSERT(fmhs_isin(&hs, &(double){-7.0}),
               "fmhs.h: fmhs_isin should return true for a inserted item: -7");
        ASSERT(fmhs_isin(&hs, &(double){259.0}),
               "fmhs.h: fmhs_isin should return true for a inserted item: 259");

        ASSERT(!fmhs_isin(&hs, &(double){-259.0}),
               "fmhs.h: fmhs_isin should return a false for a non-inserted item: -259");
        ASSERT(!fmhs_isin(&hs, &(double){3.0}),
               "fmhs.h: fmhs_isin should return a false for a non-inserted item: 3");
        ASSERT(!fmhs_isin(&hs, &(double){7.0}),
               "fmhs.h: fmhs_isin should return a false for a non-inserted item: 7");
        ASSERT(!fmhs_isin(&hs, &(double){42.0}),
               "fmhs.h: fmhs_isin should return a false for a non-inserted item: 42");
        fmhs_delete(&hs);
    });

    TEST(fmhs_remove in non exising values, {
        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(double), cmp, hash, 33, 32U);
        fmhs_insert(&hs, &(double){2.0});
        fmhs_insert(&hs, &(double){-2.0});
        fmhs_insert(&hs, &(double){-3.0});
        fmhs_insert(&hs, &(double){-7.0});
        fmhs_insert(&hs, &(double){259.0});

        int ret;
        ret = fmhs_remove(&hs, &(double){-259.0});
        ASSERT(ret < 0,
               "fmhs.h: fmhs_remove should return a negative value for a non-inserted "
               "item: -259");

        ret = fmhs_remove(&hs, &(double){3.0});
        ASSERT(ret < 0,
               "fmhs.h: fmhs_remove should return a negative value for a non-inserted "
               "item: 3");

        ret = fmhs_remove(&hs, &(double){7.0});
        ASSERT(ret < 0,
               "fmhs.h: fmhs_remove should return a negative value for a non-inserted "
               "item: 7");

        ret = fmhs_remove(&hs, &(double){42.0});
        ASSERT(ret < 0,
               "fmhs.h: fmhs_remove should return a negative value for a non-inserted "
               "item: 42");

        fmhs_delete(&hs);
    });

    TEST(fmhs_remove in exising values, {
        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(double), cmp, hash, 33, 64U);
        fmhs_insert(&hs, &(double){2.0});
        fmhs_insert(&hs, &(double){-2.0});
        fmhs_insert(&hs, &(double){-3.0});
        fmhs_insert(&hs, &(double){-7.0});
        fmhs_insert(&hs, &(double){259.0});

        int ret;
        ret = fmhs_remove(&hs, &(double){2.0});
        ASSERT(ret == 0,
               "fmhs.h: fmhs_remove should return zero for a inserted item: 2");
        ASSERT(hs.len == 4U, "fmhs.h: fmhs_remove should decrease the hs.len. Item: 2");
        ASSERT(
            !fmhs_isin(&hs, &(double){2.0}),
            "fmhs.h: After a fmhs_remove of item 2, fmhs_isin should return false for "
            "item: 2");
        ASSERT(
            fmhs_isin(&hs, &(double){-2.0}),
            "fmhs.h: After a fmhs_remove of item 2, fmhs_isin should return true for "
            "item: -2");
        ASSERT(
            fmhs_isin(&hs, &(double){-3.0}),
            "fmhs.h: After a fmhs_remove of item 2, fmhs_isin should return true for "
            "item: -3");
        ASSERT(
            fmhs_isin(&hs, &(double){-7.0}),
            "fmhs.h: After a fmhs_remove of item 2, fmhs_isin should return true for "
            "item: -7");
        ASSERT(
            fmhs_isin(&hs, &(double){259.0}),
            "fmhs.h: After a fmhs_remove of item 2, fmhs_isin should return true for "
            "item: 259");

        ret = fmhs_remove(&hs, &(double){-2.0});
        ASSERT(ret == 0,
               "fmhs.h: fmhs_remove should return zero for a inserted item: -2");
        ASSERT(hs.len == 3U,
               "fmhs.h: fmhs_remove should decrease the hs.len. Item: -2");
        ASSERT(
            !fmhs_isin(&hs, &(double){2.0}),
            "fmhs.h: After a fmhs_remove of item -2, fmhs_isin should return false for "
            "item: 2");
        ASSERT(
            !fmhs_isin(&hs, &(double){-2.0}),
            "fmhs.h: After a fmhs_remove of item -2, fmhs_isin should return false for "
            "item: -2");
        ASSERT(
            fmhs_isin(&hs, &(double){-3.0}),
            "fmhs.h: After a fmhs_remove of item -2, fmhs_isin should return true for "
            "item: -3");
        ASSERT(
            fmhs_isin(&hs, &(double){-7.0}),
            "fmhs.h: After a fmhs_remove of item -2, fmhs_isin should return true for "
            "item: -7");
        ASSERT(
            fmhs_isin(&hs, &(double){259.0}),
            "fmhs.h: After a fmhs_remove of item -2, fmhs_isin should return true for "
            "item: 259");

        ret = fmhs_remove(&hs, &(double){-3.0});
        ASSERT(ret == 0,
               "fmhs.h: fmhs_remove should return zero for a inserted item: -3");
        ASSERT(hs.len == 2U,
               "fmhs.h: fmhs_remove should decrease the hs.len. Item: -3");
        ASSERT(
            !fmhs_isin(&hs, &(double){2.0}),
            "fmhs.h: After a fmhs_remove of item -3, fmhs_isin should return false for "
            "item: 2");
        ASSERT(
            !fmhs_isin(&hs, &(double){-2.0}),
            "fmhs.h: After a fmhs_remove of item -3, fmhs_isin should return false for "
            "item: -2");
        ASSERT(
            !fmhs_isin(&hs, &(double){-3.0}),
            "fmhs.h: After a fmhs_remove of item -3, fmhs_isin should return false for "
            "item: -3");
        ASSERT(
            fmhs_isin(&hs, &(double){-7.0}),
            "fmhs.h: After a fmhs_remove of item -3, fmhs_isin should return true for "
            "item: -7");
        ASSERT(
            fmhs_isin(&hs, &(double){259.0}),
            "fmhs.h: After a fmhs_remove of item -3, fmhs_isin should return true for "
            "item: 259");

        ret = fmhs_remove(&hs, &(double){259.0});
        ASSERT(ret == 0,
               "fmhs.h: fmhs_remove should return zero for a inserted item: 259.0");
        ASSERT(hs.len == 1U,
               "fmhs.h: fmhs_remove should decrease the hs.len. Item: -259");
        ASSERT(!fmhs_isin(&hs, &(double){2.0}),
               "fmhs.h: After a fmhs_remove of item 259.0, fmhs_isin should return "
               "false for "
               "item: 2");
        ASSERT(!fmhs_isin(&hs, &(double){-2.0}),
               "fmhs.h: After a fmhs_remove of item 259.0, fmhs_isin should return "
               "false for "
               "item: -2");
        ASSERT(!fmhs_isin(&hs, &(double){-3.0}),
               "fmhs.h: After a fmhs_remove of item 259.0, fmhs_isin should return "
               "false for "
               "item: -3");
        ASSERT(fmhs_isin(&hs, &(double){-7.0}),
               "fmhs.h: After a fmhs_remove of item 259.0, fmhs_isin should return "
               "true for "
               "item: -7");
        ASSERT(!fmhs_isin(&hs, &(double){259.0}),
               "fmhs.h: After a fmhs_remove of item 259.0, fmhs_isin should return "
               "false for "
               "item: 259");

        ret = fmhs_remove(&hs, &(double){-7});
        ASSERT(ret == 0,
               "fmhs.h: fmhs_remove should return zero for a inserted item: -7");
        ASSERT(hs.len == 0U,
               "fmhs.h: fmhs_remove should decrease the hs.len. Item: -7");
        ASSERT(
            !fmhs_isin(&hs, &(double){2.0}),
            "fmhs.h: After a fmhs_remove of item -7, fmhs_isin should return false for "
            "item: 2");
        ASSERT(
            !fmhs_isin(&hs, &(double){-2.0}),
            "fmhs.h: After a fmhs_remove of item -7, fmhs_isin should return false for "
            "item: -2");
        ASSERT(
            !fmhs_isin(&hs, &(double){-3.0}),
            "fmhs.h: After a fmhs_remove of item -7, fmhs_isin should return false for "
            "item: -3");
        ASSERT(
            !fmhs_isin(&hs, &(double){-7.0}),
            "fmhs.h: After a fmhs_remove of item -7, fmhs_isin should return false for "
            "item: -7");
        ASSERT(!fmhs_isin(&hs, &(double){259.0}),
               "fmhs.h: After a fmhs_remove of item -7,, fmhs_isin should return false "
               "for "
               "item: 259");
        fmhs_delete(&hs);
    });

    TEST(fmhs_iter_begin, {
        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(double), cmp, hash, 33, 256U);
        ASSERT(fmhs_iter_begin(&hs) == NULL,
               "fmhs.h: fmhs_iter_begin should return NULL in a new hashset");

        fmhs_insert(&hs, &(double){2.04});
        fmhs_remove(&hs, &(double){2.04});
        ASSERT(fmhs_iter_begin(&hs) == NULL,
               "fmhs.h: fmhs_iter_begin should return NULL in a empty hashset");

        fmhs_insert(&hs, &(double){2.04});
        ASSERT(cmp(fmhs_iter_begin(&hs), &(double){2.04}, sizeof(double)) == 0,
               "fmhs.h: fmhs_iter_begin should return the correct value in a hashset "
               "with only one element");

        fmhs_insert(&hs, &(double){-223.0});

        ASSERT(cmp(fmhs_iter_begin(&hs), &(double){2.04}, sizeof(double)) == 0 ||
                   cmp(fmhs_iter_begin(&hs), &(double){-223.0}, sizeof(double)) == 0,
               "fmhs.h: fmhs_iter_begin should return the correct value in a hashset "
               "with more than one element");
        fmhs_delete(&hs);
    });

    TEST(fmhs_iter_next, {
        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(double), cmp, hash, 33, 256U);
        fmhs_insert(&hs, &(double){-223.0});
        fmhs_insert(&hs, &(double){-444.0});

        const double* it = NULL;
        for (it = fmhs_iter_begin(&hs); it != NULL; it = fmhs_iter_next(&hs)) {
            ASSERT(
                cmp(it, &(double){-223.0}, sizeof(double)) == 0 ||
                    cmp(it, &(double){-444.0}, sizeof(double)) == 0,
                "fmhs.h: fmhs_iter_next should return the correct value in a hashset "
                "with more than one element");
        }
        fmhs_delete(&hs);
    });

    TEST(fmhs_insert causes realloc, {
        ASSERT(FMHS_REALLOC_THRESHOLD <= 0.75,
               "fmhs.h: Assumption error, update the test contex");
        ASSERT(FMHS_GROWTH_RATE == 2,
               "fmhs.h: Assumption error, update the test contex");

        fmhs_t hs = {0};
        fmhs_new(&hs, sizeof(double), cmp, hash, 33, 1U);
        fmhs_insert(&hs, &(double){-223.0});
        fmhs_insert(&hs, &(double){-444.0});

        ASSERT(fmhs_isin(&hs, &(double){-223.0}),
               "fmhs.h: After a fmhs_insert that causes a realloc hs should have all "
               "itens "
               "item: -223");
        ASSERT(fmhs_isin(&hs, &(double){-444.0}),
               "fmhs.h: After a fmhs_insert that causes a realloc hs should have all "
               "itens "
               "item: -444");

        ASSERT(hs.len == 2U, "fmhs.h: After a fmhs_insert that causes a realloc the "
                             "len must be correct.");

        ASSERT(hs.cap == 2U, "fmhs.h: After a fmhs_insert that causes a realloc the "
                             "len must be correct.");

        ASSERT(hs.size_item == sizeof(double),
               "fmhs.h: After a fmhs_insert that causes a realloc the hs.size_item "
               "must be correct");

        ASSERT(hs.eqf == cmp, "fmhs.h: After a fmhs_insert that causes a realloc the "
                              "hs.eqf must be correct");

        ASSERT(hs.hashf == hash, "fmhs.h: After a fmhs_insert that causes a realloc "
                                 "the hs.hashf must be correct");

        fmhs_delete(&hs);
    });

    return 0;
}
#endif
