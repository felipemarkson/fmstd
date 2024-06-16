#ifndef FMDA_H
#define FMDA_H

#include <assert.h> // assert
#include <stddef.h> // NULL
#include <stdint.h> // SIZE_MAX
#include <stdlib.h> // realloc, free
#include <string.h> // memmove

#define FMDA_GROWTH 2U
#define fmda_push(ptrda, value)                                                        \
    do {                                                                               \
        assert(((ptrda)->len <= SIZE_MAX) &&                                           \
               "fmda_push: Invalid push. Too much elements");                          \
        if (((ptrda)->len + 1U) > (ptrda)->cap) {                                      \
            if ((ptrda)->cap == 0U) {                                                  \
                (ptrda)->data = NULL;                                                  \
                (ptrda)->cap  = 1U;                                                    \
            }                                                                          \
            (ptrda)->data = realloc((ptrda)->data, sizeof(*(ptrda)->data) *            \
                                                       (ptrda)->cap * FMDA_GROWTH);    \
            assert((ptrda)->data && "fmda_push: Cannot allocate more memory");         \
            (ptrda)->cap *= FMDA_GROWTH;                                               \
        }                                                                              \
        (ptrda)->data[(ptrda)->len] = (value);                                         \
        (ptrda)->len++;                                                                \
    } while (0)

#define fmda_concat(ptr_one, const_ptr_other)                                          \
    do {                                                                               \
        if ((ptr_one)->len == 0U && (const_ptr_other)->len == 0U)                      \
            break;                                                                     \
                                                                                       \
        if ((ptr_one)->cap < ((ptr_one)->len + (const_ptr_other)->len)) {              \
            if (((ptr_one)->len + (const_ptr_other)->len) % 2U == 0) {                 \
                (ptr_one)->data = realloc(                                             \
                    (ptr_one)->data, sizeof(*(ptr_one)->data) *                        \
                                         ((ptr_one)->len + (const_ptr_other)->len));   \
                assert((ptr_one)->data && "fmda_concat: Cannot allocate more memory"); \
                (ptr_one)->cap = (ptr_one)->len + (const_ptr_other)->len;              \
            } else {                                                                   \
                (ptr_one)->data =                                                      \
                    realloc((ptr_one)->data,                                           \
                            sizeof(*(ptr_one)->data) *                                 \
                                ((ptr_one)->len + (const_ptr_other)->len + 1U));       \
                assert((ptr_one)->data && "fmda_concat: Cannot allocate more memory"); \
                (ptr_one)->cap = (ptr_one)->len + (const_ptr_other)->len + 1U;         \
            }                                                                          \
        }                                                                              \
        memmove((ptr_one)->data + (ptr_one)->len, (const_ptr_other)->data,             \
                sizeof(*(ptr_one)->data) * ((const_ptr_other)->len));                  \
        (ptr_one)->len += (const_ptr_other)->len;                                      \
    } while (0)

#define fmda_free(ptr)                                                                 \
    do {                                                                               \
        free((ptr)->data);                                                             \
        (ptr)->data = NULL;                                                            \
        (ptr)->len  = 0U;                                                              \
        (ptr)->cap  = 0U;                                                              \
    } while (0)

#define fmda_at(ptr, index) (ptr)->data[index]

#endif // FMDA_H

// TESTING
#ifdef FMDA_TEST
#include <stdio.h>

typedef struct {
    int* data;
    size_t len;
    size_t cap;
} mytype;

int main(void) {
    mytype one = {0};

    printf("======== TESTING: fmda_push/fmda_at\n");
    fmda_push(&one, 1);
    fmda_push(&one, 2);
    fmda_push(&one, 3);
    fmda_push(&one, 5);
    fmda_push(&one, 8);

    assert(one.len == 5 && "one.len != 5");
    assert(one.cap == 8 && "one.len != 8");
    assert(fmda_at(&one, 0) == 1 && "[0] != 1");
    assert(fmda_at(&one, 1) == 2 && "[2] != 2");
    assert(fmda_at(&one, 2) == 3 && "[3] != 3");
    assert(fmda_at(&one, 3) == 5 && "[4] != 5");
    assert(fmda_at(&one, 4) == 8 && "[5] != 8");
    printf("======== TESTING: OK!\n");

    printf("======== TESTING fmda_concat\n");
    mytype other = {0};
    fmda_push(&other, 13);
    fmda_push(&other, 21);
    fmda_push(&other, 34);
    fmda_push(&other, 55);
    fmda_push(&other, 89);

    fmda_concat(&one, &other);
    assert(fmda_at(&one, 0) == 1 && "[0]  != 1");
    assert(fmda_at(&one, 1) == 2 && "[2]  != 2");
    assert(fmda_at(&one, 2) == 3 && "[3]  != 3");
    assert(fmda_at(&one, 3) == 5 && "[4]  != 5");
    assert(fmda_at(&one, 4) == 8 && "[5]  != 8");

    assert(fmda_at(&one, 5) == 13 && "[6]  != 13");
    assert(fmda_at(&one, 6) == 21 && "[7]  != 21");
    assert(fmda_at(&one, 7) == 34 && "[8]  != 34");
    assert(fmda_at(&one, 8) == 55 && "[9]  != 55");
    assert(fmda_at(&one, 9) == 89 && "[10] != 89");

    assert(fmda_at(&other, 0) == 13 && "[0]  != 13");
    assert(fmda_at(&other, 1) == 21 && "[1]  != 21");
    assert(fmda_at(&other, 2) == 34 && "[2]  != 34");
    assert(fmda_at(&other, 3) == 55 && "[3]  != 55");
    assert(fmda_at(&other, 4) == 89 && "[4]  != 89");

    assert(one.len == 10 && "one.len != 10");
    assert(one.cap == 10 && "one.cap != 10");
    assert(other.len == 5 && "other.len != 5");
    assert(other.cap == 8 && "other.cap != 8");
    printf("======== TESTING: OK!\n");

    fmda_free(&one);
    fmda_free(&other);
    return 0;
}
#endif // FMDA_TEST
