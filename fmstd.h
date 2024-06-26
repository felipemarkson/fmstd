/*
Created automatically.
fmstd.h - Felipe Monteiro's C standard library

Check the [repostiory](https://github.com/felipemarkson/fmstd) for more details.
---
LICENSE

MIT No Attribution

Copyright 2023 Felipe M. dos S. Monteiro

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
---
*/
/* dependency - Dependecy tree definitions */
#ifdef IMPLEMENT_FMALL
#define IMPLEMENT_FMSTRV
#define IMPLEMENT_FMDSTR
#define IMPLEMENT_FMHASHMAP
#define INCLUDE_FMDARRAY
#define INCLUDE_FMCOMMON
#endif

#ifdef IMPLEMENT_FMSTRV
#define IMPLEMENT_FMDSTR
#define INCLUDE_FMSTRV
#endif

#ifdef IMPLEMENT_FMDSTR
#define INCLUDE_FMDSTR
#endif

#ifdef IMPLEMENT_FMHASHMAP
#define INCLUDE_FMHASHMAP
#endif

#ifdef INCLUDE_FMSTRV
#define INCLUDE_FMDSTR
#endif

#ifdef INCLUDE_FMDSTR
#define INCLUDE_FMDARRAY
#endif

#ifdef INCLUDE_FMHASHMAP
#define INCLUDE_FMCOMMON
#endif

#ifdef INCLUDE_FMDARRAY
#define INCLUDE_FMCOMMON
#endif
/* ends of dependency - Dependecy tree definitions */
/* common - Common definitions used in all library */
#ifdef INCLUDE_FMCOMMON
#ifndef FMCOMMON_H
#define FMCOMMON_H

#ifndef NULL
#include <stddef.h>
#ifndef NULL
#error "Need to define NULL"
#endif
#endif

#ifndef FMREALLOC
#include <stdlib.h>
#define FMREALLOC realloc
#endif

#ifndef FMFREE
#include <stdlib.h>
#define FMFREE free
#endif

#ifndef FMSTRLEN
#include <string.h>
#define FMSTRLEN strlen
#endif

#ifndef FMMEMCPY
#include <string.h>
#define FMMEMCPY memcpy
#endif

#ifndef FMMEMSET
#include <string.h>
#define FMMEMSET memset
#endif

#ifndef FMSIZE
#include <stddef.h>
#define FMSIZE size_t
#include <stdint.h>
#define FMSIZE_MAX SIZE_MAX
#endif

#ifndef FMBOOL
#include <stdbool.h>
#define FMBOOL bool
#define FMTRUE true
#define FMFALSE false
#endif

#ifndef FMBYTE
#include <stdint.h>
#define FMBYTE uint8_t
#endif

#ifndef FMDATAPTR
#include <stdint.h>
#define FMDATAPTR uintptr_t
#endif

#ifndef FMASSERT
#include <assert.h>
#define FMASSERT assert
#endif

#endif
#endif
/* ends of common - Common definitions used in all library */
/* darray - Generic Dynamic Array macros */
#ifdef INCLUDE_FMDARRAY

#ifndef FMDARRAY_INCLUDED
#define FMDARRAY_INCLUDED

#ifndef FMDARRAY_INITAL_CAPACITY
#define FMDARRAY_INITAL_CAPACITY 8
#endif
#ifndef FMDARRAY_GROWTH_RATE
#define FMDARRAY_GROWTH_RATE 2
#endif
/*
### Members:

- `type elems`      : (`?`) Allocated array;
- `FMSIZE len`      : Number of the elements in the array (lenght);
- `FMSIZE capacity` : Array's capacity to store elements without reallocation;
- `FMBOOL error`    : Indicates if the array is in error state.

### Memory Managment:

Use `FMFREE` in `elems` to deallocate the memory. You can also use
`fmdarray_free` to have a nicer deallocation.

`NOTE:` If the elements are also dynamic-allocated, the `FMFREE` and
`fmdarray_free` will NOT deallocate these elements.

`NOTE:` Set the `len` to zero to zeroing the dynamic string (not deallocate).

`NOTE:` Zero-initilized are valid.
*/
#define fmdarray_t(type)                                                               \
    struct {                                                                           \
        type* elems;                                                                   \
        FMSIZE len;                                                                    \
        FMSIZE capacity;                                                               \
        FMBOOL error;                                                                  \
    }

/*
Realloc and set the capacity of a darray. If the new capacity is less than the darray's
lenght, then the length is also set to the new capacity.
*/
#define fmdarray_set_capacity(ptr_darray, new_capacity)                                \
    do {                                                                               \
        void* new_elems;                                                               \
        if ((ptr_darray)->capacity == (new_capacity))                                  \
            break;                                                                     \
        new_elems = FMREALLOC((ptr_darray)->elems,                                     \
                              (new_capacity) * sizeof(*((ptr_darray)->elems)));        \
        if (new_elems == NULL) {                                                       \
            (ptr_darray)->error = FMTRUE;                                              \
            break;                                                                     \
        }                                                                              \
        (ptr_darray)->elems    = new_elems;                                            \
        (ptr_darray)->capacity = (new_capacity);                                       \
        if ((ptr_darray)->len > (new_capacity))                                        \
            (ptr_darray)->len = (new_capacity);                                        \
    } while (0)

/*
Deallocated (free) the dynamic memory allocad. Same effect of FMFREE, but it zeroing the
darray.
*/
#define fmdarray_free(ptr_darray)                                                      \
    do {                                                                               \
        FMFREE((ptr_darray)->elems);                                                   \
        (ptr_darray)->elems    = NULL;                                                 \
        (ptr_darray)->len      = 0;                                                    \
        (ptr_darray)->capacity = 0;                                                    \
    } while (0)

/* Push (by copying)  the new element into the darray. */
#define fmdarray_push(ptr_darray, new_elem)                                            \
    do {                                                                               \
        FMSIZE len = (ptr_darray)->len;                                                \
        FMSIZE cap = (ptr_darray)->capacity;                                           \
        if (len < cap) {                                                               \
            (ptr_darray)->elems[len] = new_elem;                                       \
            ++(ptr_darray)->len;                                                       \
        } else {                                                                       \
            FMSIZE new_cap =                                                           \
                cap ? FMDARRAY_GROWTH_RATE * cap : FMDARRAY_INITAL_CAPACITY;           \
            fmdarray_set_capacity(ptr_darray, new_cap);                                \
            if ((ptr_darray)->error)                                                   \
                break;                                                                 \
            (ptr_darray)->elems[len] = new_elem;                                       \
            ++(ptr_darray)->len;                                                       \
        }                                                                              \
    } while (0)

/*
Removes the last element of the darray and copy him to the out_ptr_elem(?) if
out_ptr_elem is not NULL
*/
#define fmdarray_pop(ptr_darray, out_ptr_elem)                                         \
    do {                                                                               \
        FMSIZE len = (ptr_darray)->len;                                                \
        FMSIZE cap = (ptr_darray)->capacity;                                           \
        if (len == 0 || cap == 0) {                                                    \
            (ptr_darray)->error = FMTRUE;                                              \
        } else {                                                                       \
            if (out_ptr_elem != NULL)                                                  \
                *out_ptr_elem = (ptr_darray)->elems[len - 1];                          \
            --(ptr_darray)->len;                                                       \
        }                                                                              \
    } while (0)

/*
Removes the element at the index and moves the remaning elements to the left. Copy the
elemnt to the out_ptr_elem(?) if out_ptr_elem is not NULL.
*/
#define fmdarray_drop(ptr_darray, index, out_ptr_elem)                                 \
    do {                                                                               \
        FMSIZE len = (ptr_darray)->len;                                                \
        FMSIZE i   = index;                                                            \
        FMSIZE j;                                                                      \
        if (i >= len) {                                                                \
            (ptr_darray)->error = FMTRUE;                                              \
        } else {                                                                       \
            if (out_ptr_elem != NULL)                                                  \
                *out_ptr_elem = (ptr_darray)->elems[i];                                \
            for (j = i + 1; j < len; j++)                                              \
                (ptr_darray)->elems[j - 1] = (ptr_darray)->elems[j];                   \
            --(ptr_darray)->len;                                                       \
        }                                                                              \
    } while (0)

/* Moves the remaning elements to the right and insert a element at the index */
#define fmdarray_insert(ptr_darray, index, new_elem)                                   \
    do {                                                                               \
        FMSIZE intial_len = (ptr_darray)->len;                                         \
        FMSIZE i          = index;                                                     \
        if (i > intial_len) {                                                          \
            (ptr_darray)->error = FMTRUE;                                              \
        } else {                                                                       \
            if ((intial_len + 1) > (ptr_darray)->capacity)                             \
                fmdarray_push(ptr_darray, (ptr_darray)->elems[intial_len - 1]);        \
            else                                                                       \
                ++(ptr_darray)->len;                                                   \
            if ((ptr_darray)->error)                                                   \
                break;                                                                 \
            for (FMSIZE j = intial_len - 1; j >= i; j--)                               \
                (ptr_darray)->elems[j + 1] = (ptr_darray)->elems[j];                   \
            (ptr_darray)->elems[i] = new_elem;                                         \
        }                                                                              \
    } while (0)
/* Insert the sec_ptr_darray's elements into the first one. */
#define fmdarray_concat(first_ptr_darray, sec_ptr_darray)                              \
    do {                                                                               \
        FMSIZE index;                                                                  \
        FMSIZE actual_len = (first_ptr_darray)->len;                                   \
        FMSIZE actual_cap = (first_ptr_darray)->capacity;                              \
        FMSIZE req_cap    = actual_len + (sec_ptr_darray)->len;                        \
        if (req_cap > actual_cap)                                                      \
            fmdarray_set_capacity(first_ptr_darray, req_cap);                          \
        if ((first_ptr_darray)->error)                                                 \
            break;                                                                     \
        for (index = actual_len; index < req_cap; index++)                             \
            (first_ptr_darray)->elems[index] =                                         \
                (sec_ptr_darray)->elems[index - actual_len];                           \
        (first_ptr_darray)->len = req_cap;                                             \
    } while (0)

/* Reverse the first_ptr_darray inplace. */
#define fmdarray_reverse(first_ptr_darray, type)                                       \
    do {                                                                               \
        type temp;                                                                     \
        FMSIZE i, j;                                                                   \
        j = ((first_ptr_darray)->len - 1);                                             \
        for (i = 0; i < ((first_ptr_darray)->len / 2); i++) {                          \
            temp                         = (first_ptr_darray)->elems[i];               \
            (first_ptr_darray)->elems[i] = (first_ptr_darray)->elems[j];               \
            (first_ptr_darray)->elems[j] = temp;                                       \
            --j;                                                                       \
        }                                                                              \
    } while (0)

/* Check if two darrays has the same elemens */
#define fmdarray_eq(first_ptr_darray, second_ptr_darray, out_ptr_bool)                 \
    do {                                                                               \
        FMSIZE i;                                                                      \
        *(out_ptr_bool) = FMFALSE;                                                     \
        if ((first_ptr_darray)->len != (second_ptr_darray)->len) {                     \
            break;                                                                     \
        }                                                                              \
        for (i = 0; i < (first_ptr_darray)->len; ++i) {                                \
            if ((first_ptr_darray)->elems[i] == (second_ptr_darray)->elems[i])         \
                *(out_ptr_bool) = FMTRUE;                                              \
            else {                                                                     \
                *(out_ptr_bool) = FMFALSE;                                             \
                break;                                                                 \
            }                                                                          \
        }                                                                              \
    } while (0)

/* Find the fist occurrence of the second darray in the first one. */
#define fmdarray_find(first_ptr_darray, second_ptr_darray, out_ptr_bool,               \
                      out_ptr_index)                                                   \
    do {                                                                               \
        FMSIZE i, j;                                                                   \
        *(out_ptr_bool) = FMFALSE;                                                     \
        for (i = 0; i < (first_ptr_darray)->len; i++) {                                \
            FMBOOL found = FMTRUE;                                                     \
            FMSIZE k     = i;                                                          \
            for (j = 0; j < (second_ptr_darray)->len; j++) {                           \
                if ((k < (first_ptr_darray)->len) &&                                   \
                    ((first_ptr_darray)->elems[k] == (second_ptr_darray)->elems[j])) { \
                    ++k;                                                               \
                } else {                                                               \
                    found = FMFALSE;                                                   \
                    break;                                                             \
                }                                                                      \
            }                                                                          \
            if (found) {                                                               \
                *(out_ptr_index) = i;                                                  \
                *(out_ptr_bool)  = FMTRUE;                                             \
                break;                                                                 \
            }                                                                          \
        }                                                                              \
    } while (0)

#endif
#endif
/* ends of darray - Generic Dynamic Array macros */
/* dstr - Dynamic string type and functions */
#ifdef INCLUDE_FMDSTR

#ifndef FMDSTR_INCLUDED
#define FMDSTR_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/*
### Members:

- `char* elems`      : (`?`) Allocated array;
- `FMSIZE len`      : Number of the elements in the array (lenght);
- `FMSIZE capacity` : Array's capacity to store elements without reallocation;
- `FMBOOL error`    : Indicates if the array is in error state.

### Memory Managment:

Use `FMFREE` in `elems` to deallocate the memory. You can also use
`darray.h/fmdarray_free` to have a nicer deallocation.

`NOTE:` If the elements are also dynamic-allocated, the `FMFREE` and
`darray.h/fmdarray_free` will NOT deallocate these elements.

`NOTE:` Set the `len` to zero to zeroing the dynamic string (not deallocate).

`NOTE:` Zero-initilized `fmdstr_t` are valid.
*/
typedef fmdarray_t(char) fmdstr_t;

/* Creates a new dynamic string by copying an `'\0'`-terminated array of `char` */
fmdstr_t fmdstr_from_cstr(const char null_terminated_cstr[static 1]);

/* Push (by copying) an array of `char` into the dynamic string */
void fmdstr_push_array(fmdstr_t string[static 1], FMSIZE array_len,
                       const char array[static array_len]);

/*
It allocates a new `'\0'`-terminated array of `char` and copy dynamic string's
characters into it. The user is reponsible to free the returned array
*/
char* fmdstr_to_cstr(const fmdstr_t string[static 1]);

/* Push (by copying) the second dynamic string into the first one */
void fmdstr_concat(fmdstr_t first[static 1], const fmdstr_t second[static 1]);

/* Reverts the dynamic string */
void fmdstr_reverse(fmdstr_t dstr[static 1]);

/* TODO
    from_wcstr
    to_wcstr
    replace
    format
*/

#ifdef __cplusplus
}
#endif
#endif
#endif

/* darray - Implementation */
#ifdef IMPLEMENT_FMDSTR

void fmdstr_push_array(fmdstr_t string[static 1], FMSIZE array_len,
                       const char array[static array_len]) {
    FMSIZE initial_len = string->len;
    FMSIZE req_cap     = array_len + initial_len;
    FMSIZE i;
    if ((req_cap > string->capacity))
        fmdarray_set_capacity(string, FMDARRAY_GROWTH_RATE * req_cap);
    if (string->error || (string->elems == NULL)) {
        string->error = FMTRUE;
        return;
    }

    for (i = 0; i < array_len; i++)
        string->elems[initial_len + i] = array[i];
    string->len = req_cap;
}

fmdstr_t fmdstr_from_cstr(const char null_terminated_cstr[static 1]) {
    fmdstr_t out = {0};
    if (null_terminated_cstr == NULL)
        return out;
    FMSIZE len = FMSTRLEN(null_terminated_cstr);
    if (len == 0)
        return out;
    fmdstr_push_array(&out, len, null_terminated_cstr);
    return out;
}

char* fmdstr_to_cstr(const fmdstr_t string[static 1]) {
    char* out;
    FMSIZE i;
    out = FMREALLOC(NULL, (string->len + 1) * sizeof(char));
    if (out == NULL)
        return out;
    for (i = 0; i < string->len; i++)
        out[i] = string->elems[i];
    out[string->len] = '\0';
    return out;
}

void fmdstr_concat(fmdstr_t first[static 1], const fmdstr_t second[static 1]) {
    fmdstr_push_array(first, second->len, second->elems);
}

void fmdstr_reverse(fmdstr_t dstr[static 1]) { fmdarray_reverse(dstr, char); }

#endif
/* end of dstr - Dynamic string type and functions */
/* strv - String view type and functions */
#ifdef INCLUDE_FMSTRV
#ifndef FMSTRV_INCLUDED
#define FMSTRV_INCLUDED

typedef struct {
    const char* elems;
    const FMSIZE len;
} fmstrv_t;

void fmstrv_from_dstr(const fmdstr_t dstr[static 1], fmstrv_t out[static 1],
                      FMSIZE begin, FMSIZE len);

void fmstrv_from_cstr(const char null_terminated_cstr[static 1], fmstrv_t out[static 1],
                      FMSIZE begin, FMSIZE len);

void fmstrv_from_array(FMSIZE array_size, const char array[static array_size],
                       fmstrv_t out[static 1], FMSIZE begin, FMSIZE len);

FMBOOL fmstrv_find(const fmstrv_t strv[static 1], const fmstrv_t token[static 1],
                   FMSIZE out_index[static 1]);

char* fmstrv_to_cstr(const fmstrv_t strv[static 1]);
fmdstr_t fmstrv_to_dstr(const fmstrv_t strv[static 1]);

FMBOOL fmstrv_eq(const fmstrv_t strv[static 1], const fmstrv_t other[static 1]);

/* TODO
    split
    endswith
    startswith
*/
#endif
#endif

/* strv - Implementation */
#ifdef IMPLEMENT_FMSTRV
void fmstrv_from_dstr(const fmdstr_t dstr[static 1], fmstrv_t out[static 1],
                      FMSIZE begin, FMSIZE len) {
    char** ptr        = (char**)(&(out->elems));
    FMSIZE* inner_len = (FMSIZE*)(&(out->len));
    if ((begin + len) <= dstr->len) {
        *ptr       = (&(dstr->elems[begin]));
        *inner_len = len;
    } else {
        *ptr       = NULL;
        *inner_len = 0;
    }
}

void fmstrv_from_array(FMSIZE array_size, const char array[static array_size],
                       fmstrv_t out[static 1], FMSIZE begin, FMSIZE len) {
    char** ptr        = (char**)(&(out->elems));
    FMSIZE* inner_len = (FMSIZE*)(&(out->len));
    if ((begin + len) <= array_size) {
        *ptr       = (char*)(&(array[begin]));
        *inner_len = len;
    } else {
        *ptr       = NULL;
        *inner_len = 0;
    }
}

void fmstrv_from_cstr(const char null_terminated_cstr[static 1], fmstrv_t out[static 1],
                      FMSIZE begin, FMSIZE len) {
    char** ptr        = (char**)(&(out->elems));
    FMSIZE* inner_len = (FMSIZE*)(&(out->len));

    FMSIZE size = FMSTRLEN(null_terminated_cstr);
    if ((begin + len) <= size) {
        *ptr       = (char*)(&(null_terminated_cstr[begin]));
        *inner_len = len;
    } else {
        *ptr       = NULL;
        *inner_len = 0;
    }
}

FMBOOL fmstrv_find(const fmstrv_t strv[static 1], const fmstrv_t token[static 1],
                   FMSIZE out_index[static 1]) {
    FMBOOL out;
    fmdarray_find(strv, token, &out, out_index);
    return out;
}

char* fmstrv_to_cstr(const fmstrv_t strv[static 1]) {
    char* out = FMREALLOC(NULL, (strv->len + 1) * sizeof(char));
    FMSIZE i;
    if (out == NULL)
        return out;
    for (i = 0; i < strv->len; ++i)
        out[i] = strv->elems[i];
    out[strv->len] = '\0';
    return out;
}

fmdstr_t fmstrv_to_dstr(const fmstrv_t strv[static 1]) {
    fmdstr_t out    = {0};
    FMSIZE capacity = 2 * (strv->len) * sizeof(char);
    FMSIZE i;
    out.elems = FMREALLOC(NULL, capacity);
    if (out.elems == NULL) {
        out.error = FMTRUE;
        return out;
    }
    for (i = 0; i < strv->len; ++i)
        out.elems[i] = strv->elems[i];
    out.capacity = capacity;
    out.len      = strv->len;
    return out;
}

FMBOOL fmstrv_eq(const fmstrv_t strv[static 1], const fmstrv_t other[static 1]) {
    FMBOOL out;
    fmdarray_eq(strv, other, &out);
    return out;
}

#endif

/* end of strv - String view type and functions */
/* fmhm_t */

// TODO: TESTING ON BIGENDIAN
#ifdef INCLUDE_FMHASHMAP

#ifndef FMHASHMAP_INCLUDED
#define FMHASHMAP_INCLUDED

typedef FMBOOL (*fmcompkey_f)(const void* ptr_key1, const void* ptr_key2);
typedef FMSIZE (*fmhash_f)(const void* ptr_key);

typedef struct {
    unsigned char* data;
    FMSIZE capacity;
    fmcompkey_f eqkeyfunc;
    fmhash_f hashfunc;
    FMSIZE size_key;
    FMSIZE size_value;
    FMSIZE size_data;
    FMSIZE iter_index;
} fmhm_t;

typedef struct {
    const void* ptr_key;
    const void* ptr_value;
} fmhm_pair_t;

int fmhm_init_inner(FMSIZE size_key, FMSIZE size_value, fmhm_t hs[static 1],
                    fmcompkey_f eqkeyfunc, fmhash_f hashfunc);

void fmhm_deinit(fmhm_t hs[static 1]);

#define fmhm_init(K, V, hsptr, eqkeyfunc, hashfunc)                                    \
    fmhm_init_inner(sizeof(K), sizeof(V), (hsptr), (eqkeyfunc), (hashfunc))

fmhm_pair_t fmhm_get(fmhm_t hs[static 1], const void* ptr_key);
int fmhm_insert(fmhm_t hs[static 1], const void* ptr_key, const void* ptr_value);

#endif

#ifdef IMPLEMENT_FMHASHMAP
#define FMHASHMAP_INIT_CAP 2U

void* fmhm_ptr_key_by_index(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    return &(hs->data[data_index]);
}

void fmhm_insert_key(fmhm_t hs[static 1], FMSIZE index, const void* ptr_key) {
    unsigned char* dest      = fmhm_ptr_key_by_index(hs, index);
    const unsigned char* src = ptr_key;
    for (FMSIZE i = 0; i < hs->size_key; i++)
        dest[i] = src[i];
}

void* fmhm_ptr_value_by_index(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    return &(hs->data[data_index]) + hs->size_key;
}

void fmhm_insert_value(fmhm_t hs[static 1], FMSIZE index, const void* ptr_value) {
    unsigned char* dest      = fmhm_ptr_value_by_index(hs, index);
    const unsigned char* src = ptr_value;
    for (FMSIZE i = 0; i < hs->size_value; i++)
        dest[i] = src[i];
}

FMBOOL fmhm_istaken(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    return *(&(hs->data[data_index]) + hs->size_key + hs->size_value);
}

void fmhm_set_taken(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    *(&(hs->data[data_index]) + hs->size_key + hs->size_value) = 1;
}
void fmhm_unset_taken(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    *(&(hs->data[data_index]) + hs->size_key + hs->size_value) = 0;
}

int fmhm_init_inner(FMSIZE size_key, FMSIZE size_value, fmhm_t hs[static 1],
                    fmcompkey_f eqkeyfunc, fmhash_f hashfunc) {

    FMSIZE size_data    = FMHASHMAP_INIT_CAP * (size_key + size_value + 1U);
    unsigned char* data = FMREALLOC(NULL, size_data);
    if (data == NULL) {
        FMFREE(data);
        return -1;
    }
    for (FMSIZE i = 0; i < size_data; i++)
        data[i] = 0;

    hs->data       = data;
    hs->capacity   = FMHASHMAP_INIT_CAP;
    hs->eqkeyfunc  = eqkeyfunc;
    hs->hashfunc   = hashfunc;
    hs->size_key   = size_key;
    hs->size_value = size_value;
    hs->size_data  = size_data;
    hs->iter_index = 0;
    return 0;
}

fmhm_pair_t fmhm_get(fmhm_t hs[static 1], const void* ptr_key) {
    FMSIZE i     = hs->hashfunc(ptr_key) % hs->capacity;
    FMSIZE count = 0;
    while (count < hs->capacity) {
        while (!fmhm_istaken(hs, i)) {
            ++i;
            ++count;
            if (count >= hs->capacity)
                return (fmhm_pair_t){NULL, NULL};
            if (i >= hs->capacity)
                i = 0;
        }

        const void* ptr_otherkey = fmhm_ptr_key_by_index(hs, i);
        if (hs->eqkeyfunc(ptr_otherkey, ptr_key))
            return (fmhm_pair_t){ptr_otherkey, fmhm_ptr_value_by_index(hs, i)};
        ++i;
        ++count;
        if (count >= hs->capacity)
            return (fmhm_pair_t){NULL, NULL};
        if (i >= hs->capacity)
            i = 0;
    }
    return (fmhm_pair_t){NULL, NULL};
}

int fmhm_insert(fmhm_t hs[static 1], const void* ptr_key, const void* ptr_value) {
    if (hs->capacity == 0U)
        return -1;

    FMSIZE i     = hs->hashfunc(ptr_key) % hs->capacity;
    FMSIZE count = 0;

    while (fmhm_istaken(hs, i)) {
        ++i;
        ++count;
        if (count >= hs->capacity)
            return -1;
        if (i >= hs->capacity)
            i = 0;
    }
    fmhm_insert_key(hs, i, ptr_key);
    fmhm_insert_value(hs, i, ptr_value);
    fmhm_set_taken(hs, i);
    hs->iter_index = 0;
    return 0;
}

void fmhm_deinit(fmhm_t hs[static 1]) {
    FMFREE(hs->data);
    hs->data     = NULL;
    hs->capacity = 0;
}

void fmhm_iter_start(fmhm_t hs[static 1]) { hs->iter_index = 0; }

int fmhm_iter_next(fmhm_t hs[static 1], fmhm_pair_t out[static 1]) {
    while (hs->iter_index < hs->capacity) {
        if (fmhm_istaken(hs, hs->iter_index)) {
            out->ptr_key   = fmhm_ptr_key_by_index(hs, hs->iter_index);
            out->ptr_value = fmhm_ptr_value_by_index(hs, hs->iter_index);
            hs->iter_index++;
            return 0;
        }
        hs->iter_index++;
    }
    return -1;
}

#endif
#endif
/* end of fmhm_t*/
