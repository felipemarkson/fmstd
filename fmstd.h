/*
Automaticaly created.
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
#ifdef IMPLEMENT_FMSTRV
#define INCLUDE_FMSTRV
#endif

#ifdef IMPLEMENT_FMDSTR
#define INCLUDE_FMDSTR
#endif

#ifdef INCLUDE_FMSTRV
#define INCLUDE_FMDSTR
#endif

#ifdef INCLUDE_FMDSTR
#define INCLUDE_FMDARRAY
#endif

#ifdef INCLUDE_FMDARRAY
#define INCLUDE_FMCOMMON
#endif
/* ends of dependency - Dependecy tree definitions */
/* common - Common definitions used in all library */
#ifdef INCLUDE_FMCOMMON
#ifndef FMCOMMON_H

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

#ifndef FMSIZE
#include <stddef.h>
#define FMSIZE size_t
#endif

#ifndef FMBOOL
#define FMBOOL unsigned int
#endif

#ifndef FMTRUE
#define FMTRUE ((FMBOOL)1)
#endif

#ifndef FMFALSE
#define FMFALSE ((FMBOOL)0)
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

#define fmdarray_t(type) \
    struct {              \
        type* elems;      \
        FMSIZE len;      \
        FMSIZE capacity; \
        FMBOOL error;    \
    }

#define fmdarray_set_capacity(ptr_darray, new_capacity)                            \
    do {                                                                            \
        void* new_elems;                                                            \
        if ((ptr_darray)->capacity == (new_capacity)) break;                        \
        new_elems = FMREALLOC((ptr_darray)->elems,                                 \
                               (new_capacity) * sizeof(*((ptr_darray)->elems)));    \
        if (new_elems == NULL) {                                                    \
            (ptr_darray)->error = FMTRUE;                                          \
            break;                                                                  \
        }                                                                           \
        (ptr_darray)->elems = new_elems;                                            \
        (ptr_darray)->capacity = (new_capacity);                                    \
        if ((ptr_darray)->len > (new_capacity)) (ptr_darray)->len = (new_capacity); \
    } while (0)

#define fmdarray_free(ptr_darray)    \
    do {                              \
        FMFREE((ptr_darray)->elems); \
        (ptr_darray)->elems = NULL;   \
        (ptr_darray)->len = 0;        \
        (ptr_darray)->capacity = 0;   \
    } while (0)

#define fmdarray_push(ptr_darray, new_elem)                                   \
    do {                                                                       \
        FMSIZE len = (ptr_darray)->len;                                       \
        FMSIZE cap = (ptr_darray)->capacity;                                  \
        if (len < cap) {                                                       \
            (ptr_darray)->elems[len] = new_elem;                               \
            ++(ptr_darray)->len;                                               \
        } else {                                                               \
            FMSIZE new_cap =                                                  \
                cap ? FMDARRAY_GROWTH_RATE * cap : FMDARRAY_INITAL_CAPACITY; \
            fmdarray_set_capacity(ptr_darray, new_cap);                       \
            if ((ptr_darray)->error) break;                                    \
            (ptr_darray)->elems[len] = new_elem;                               \
            ++(ptr_darray)->len;                                               \
        }                                                                      \
    } while (0)

#define fmdarray_pop(ptr_darray, out_ptr_elem)                                     \
    do {                                                                            \
        FMSIZE len = (ptr_darray)->len;                                            \
        FMSIZE cap = (ptr_darray)->capacity;                                       \
        if (len == 0 || cap == 0) {                                                 \
            (ptr_darray)->error = FMTRUE;                                          \
        } else {                                                                    \
            if (out_ptr_elem != NULL) *out_ptr_elem = (ptr_darray)->elems[len - 1]; \
            --(ptr_darray)->len;                                                    \
        }                                                                           \
    } while (0)

#define fmdarray_drop(ptr_darray, index, out_ptr_elem)                       \
    do {                                                                      \
        FMSIZE len = (ptr_darray)->len;                                      \
        FMSIZE i = index;                                                    \
        FMSIZE j;                                                            \
        if (i >= len) {                                                       \
            (ptr_darray)->error = FMTRUE;                                    \
        } else {                                                              \
            if (out_ptr_elem != NULL) *out_ptr_elem = (ptr_darray)->elems[i]; \
            for (j = i + 1; j < len; j++)                                     \
                (ptr_darray)->elems[j - 1] = (ptr_darray)->elems[j];          \
            --(ptr_darray)->len;                                              \
        }                                                                     \
    } while (0)

#define fmdarray_insert(ptr_darray, index, out_ptr_elem)                     \
    do {                                                                      \
        FMSIZE len = (ptr_darray)->len;                                      \
        FMSIZE i = index;                                                    \
        FMSIZE j;                                                            \
        if (i >= len) {                                                       \
            (ptr_darray)->error = FMTRUE;                                    \
        } else {                                                              \
            if (out_ptr_elem != NULL) *out_ptr_elem = (ptr_darray)->elems[i]; \
            for (j = i + 1; j < len; j++)                                     \
                (ptr_darray)->elems[j - 1] = (ptr_darray)->elems[j];          \
            --(ptr_darray)->len;                                              \
        }                                                                     \
    } while (0)

#define fmdarray_concat(first_ptr_darray, sec_ptr_darray)                           \
    do {                                                                             \
        FMSIZE index;                                                               \
        FMSIZE req_cap = (first_ptr_darray)->len + (sec_ptr_darray)->len;           \
        FMSIZE actual_cap = (first_ptr_darray)->capacity;                           \
        if (req_cap > actual_cap) fmdarray_set_capacity(first_ptr_darray, req_cap); \
        if ((first_ptr_darray)->error) break;                                        \
        for (index = (first_ptr_darray)->len; index < req_cap; index++)              \
            (first_ptr_darray)->elems[index] =                                       \
                (sec_ptr_darray)->elems[index - (first_ptr_darray)->len];            \
        (first_ptr_darray)->len = req_cap;                                           \
    } while (0)

#define fmdarray_reverse(first_ptr_darray, type)                        \
    do {                                                                 \
        type temp;                                                       \
        FMSIZE i, j;                                                    \
        j = ((first_ptr_darray)->len - 1);                               \
        for (i = 0; i < ((first_ptr_darray)->len / 2); i++) {            \
            temp = (first_ptr_darray)->elems[i];                         \
            (first_ptr_darray)->elems[i] = (first_ptr_darray)->elems[j]; \
            (first_ptr_darray)->elems[j] = temp;                         \
            --j;                                                         \
        }                                                                \
    } while (0)

#define fmdarray_eq(first_ptr_darray, second_ptr_darray, out_ptr_bool)        \
    do {                                                                       \
        FMSIZE i;                                                             \
        *(out_ptr_bool) = FMFALSE;                                            \
        if ((first_ptr_darray)->len != (second_ptr_darray)->len) {             \
            break;                                                             \
        }                                                                      \
        for (i = 0; i < (first_ptr_darray)->len; ++i) {                        \
            if ((first_ptr_darray)->elems[i] == (second_ptr_darray)->elems[i]) \
                *(out_ptr_bool) = FMTRUE;                                     \
            else {                                                             \
                *(out_ptr_bool) = FMFALSE;                                    \
                break;                                                         \
            }                                                                  \
        }                                                                      \
    } while (0)

#define fmdarray_find(first_ptr_darray, second_ptr_darray, out_ptr_bool,          \
                       out_ptr_index)                                              \
    do {                                                                           \
        FMSIZE i, j;                                                              \
        *(out_ptr_bool) = FMFALSE;                                                  \
        for (i = 0; i < (first_ptr_darray)->len; i++) {                              \
            FMBOOL found = FMTRUE;                                               \
            FMSIZE k = i;                                                         \
            for (j = 0; j < (second_ptr_darray)->len; j++) {                         \
                if ((k < (first_ptr_darray)->len) &&                                 \
                    ((first_ptr_darray)->elems[k] == (second_ptr_darray)->elems[j])) { \
                    ++k;                                                           \
                } else {                                                           \
                    found = FMFALSE;                                              \
                    break;                                                         \
                }                                                                  \
            }                                                                      \
            if (found) {                                                           \
                *(out_ptr_index) = i;                                                \
                *(out_ptr_bool) = FMTRUE;                                           \
                break;                                                             \
            }                                                                      \
        }                                                                          \
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
fmdstr_t fmdstr_from_cstr(const char null_terminated_cstr[]);

/* Push (by copying) an array of `char` into the dynamic string */
void fmdstr_push_array(fmdstr_t* string, FMSIZE array_len, const char array[]);

/*
It allocates a new `'\0'`-terminated array of `char` and copy dynamic string's
characters into it. The user is reponsible to free the returned array
*/
char* fmdstr_to_cstr(const fmdstr_t* string);

/* Push (by copying) the second dynamic string into the first one */
void fmdstr_concat(fmdstr_t* first, const fmdstr_t* second);

/* Dynamic string equality comparision. Checks the lenght and the elements.*/
FMBOOL fmdstr_eq(const fmdstr_t* dstr, const fmdstr_t* other);

/*
Check if the `token` is in `dstr` storing the location in `out_index`.
Returns `FMTRUE` if found, `FMFALSE`otherwise
*/
FMBOOL fmdstr_find(const fmdstr_t* dstr, const fmdstr_t* token, FMSIZE* out_index);

/*
Check if the `token` is in `dstr` storing the location in `out_index`.
Returns `FMTRUE` if found, `FMFALSE`otherwise
*/
FMBOOL fmdstr_find_da(const fmdstr_t* dstr, const char* array_token, FMSIZE size,
                      FMSIZE* out_index);

/*
Check if the `token` is in `dstr` storing the location in `out_index`.
Returns `FMTRUE` if found, `FMFALSE`otherwise
*/
FMBOOL fmdstr_find_dc(const fmdstr_t* dstr, const char* null_terminated_token,
                      FMSIZE* out_index);

/* Reverts the dynamic string */
void fmdstr_reverse(fmdstr_t* dstr);

/* TODO
    from_wcstr
    to_wcstr
    replace
    split
    endswith
    startswith
    format
*/

#ifdef __cplusplus
}
#endif
#endif
#endif

/* darray - Implementation */
#ifdef IMPLEMENT_FMDSTR

void fmdstr_push_array(fmdstr_t* string, FMSIZE array_len, const char array[]) {
    FMSIZE req_cap = array_len + string->len;
    FMSIZE i;
    if ((req_cap > string->capacity) || (string->elems == NULL))
        fmdarray_set_capacity(string, FMDARRAY_GROWTH_RATE * req_cap);
    if (string->error) return;

    for (i = 0; i < array_len; i++) string->elems[string->len + i] = array[i];
    string->len = req_cap;
}

fmdstr_t fmdstr_from_cstr(const char null_terminated_cstr[]) {
    FMSIZE len;
    fmdstr_t out = {0};
    const char* temp;
    if (null_terminated_cstr == NULL) return out;
    temp = null_terminated_cstr;
    len = 0;
    while (*temp != '\0') {
        ++len;
        ++temp;
    }
    if (len == 0) return out;
    fmdstr_push_array(&out, len, null_terminated_cstr);
    return out;
}

char* fmdstr_to_cstr(const fmdstr_t* string) {
    char* out;
    FMSIZE i;
    out = FMREALLOC(NULL, (string->len + 1) * sizeof(char));
    if (out == NULL) return out;
    for (i = 0; i < string->len; i++) out[i] = string->elems[i];
    out[string->len] = '\0';
    return out;
}

void fmdstr_concat(fmdstr_t* first, const fmdstr_t* second) {
    fmdstr_push_array(first, second->len, second->elems);
}

FMBOOL fmdstr_find(const fmdstr_t* dstr, const fmdstr_t* token, FMSIZE* out_index) {
    FMBOOL out;
    fmdarray_find(dstr, token, &out, out_index);
    return out;
}

void fmdstr_reverse(fmdstr_t* dstr) { fmdarray_reverse(dstr, char); }

FMBOOL fmdstr_eq(const fmdstr_t* dstr, const fmdstr_t* other) {
    FMBOOL out;
    fmdarray_eq(dstr, other, &out);
    return out;
}

FMBOOL fmdstr_find_da(const fmdstr_t* dstr, const char* array_token, FMSIZE size,
                      FMSIZE* out_index) {
    fmdstr_t temp = {0};
    FMBOOL out;

    temp.elems = (char*)array_token;
    temp.capacity = size;
    temp.len = size;

    fmdarray_find(dstr, &temp, &out, out_index);
    return out;
}

FMBOOL fmdstr_find_dc(const fmdstr_t* dstr, const char* null_terminated_token,
                      FMSIZE* out_index) {
    fmdstr_t temp = {0};
    const char* temp_ptr = null_terminated_token;
    FMBOOL out;

    temp.elems = (char*)null_terminated_token;
    while (*temp_ptr != '\0') {
        ++temp.len;
        ++temp.capacity;
        ++temp_ptr;
    }

    fmdarray_find(dstr, &temp, &out, out_index);
    return out;
}
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

void fmstrv_from_dstr(const fmdstr_t* dstr, fmstrv_t* out, FMSIZE begin, FMSIZE len);

void fmstrv_from_cstr(const char* null_terminated_cstr, fmstrv_t* out, FMSIZE begin,
                      FMSIZE len);

void fmstrv_from_array(const char* array, FMSIZE size, fmstrv_t* out, FMSIZE begin,
                       FMSIZE len);

FMBOOL fmstrv_find(const fmstrv_t* strv, const fmstrv_t* token, FMSIZE* out_index);

FMBOOL fmstrv_find_dv(const fmdstr_t* dstr, const fmstrv_t* token, FMSIZE* out_index);

FMBOOL fmdstr_find_vd(const fmstrv_t* strv, const fmdstr_t* token, FMSIZE* out_index);

FMBOOL fmdstr_find_av(const char* array, FMSIZE size, const fmstrv_t* token,
                      FMSIZE* out_index);

FMBOOL fmdstr_find_va(const fmstrv_t* strv, const char* array_token, FMSIZE size,
                      FMSIZE* out_index);

FMBOOL fmdstr_find_cv(const char* null_terminated_cstr, const fmstrv_t* token,
                      FMSIZE* out_index);

FMBOOL fmdstr_find_vc(const fmstrv_t* strv, const char* null_terminated_token,
                      FMSIZE* out_index);

char* fmstrv_to_cstr(const fmstrv_t* strv);

#endif
#endif

/* strv - Implementation */
#ifdef IMPLEMENT_FMSTRV
void fmstrv_from_dstr(const fmdstr_t* dstr, fmstrv_t* out, FMSIZE begin, FMSIZE len) {
    char** ptr = (char**)(&(out->elems));
    FMSIZE* inner_len = (FMSIZE*)(&(out->len));
    if ((begin + len) <= dstr->len) {
        *ptr = (&(dstr->elems[begin]));
        *inner_len = len;
    } else {
        *ptr = NULL;
        *inner_len = 0;
    }
}

void fmstrv_from_array(const char* array, FMSIZE size, fmstrv_t* out, FMSIZE begin,
                       FMSIZE len) {
    char** ptr = (char**)(&(out->elems));
    FMSIZE* inner_len = (FMSIZE*)(&(out->len));
    if ((begin + len) <= size) {
        *ptr = (char*)(&(array[begin]));
        *inner_len = len;
    } else {
        *ptr = NULL;
        *inner_len = 0;
    }
}

void fmstrv_from_cstr(const char* null_terminated_cstr, fmstrv_t* out, FMSIZE begin,
                      FMSIZE len) {
    char** ptr = (char**)(&(out->elems));
    FMSIZE* inner_len = (FMSIZE*)(&(out->len));
    char* temp = (char*)null_terminated_cstr;
    FMSIZE size = 0;
    while (*temp != '\0') {
        ++size;
        ++temp;
    }
    if ((begin + len) <= size) {
        *ptr = (char*)(&(null_terminated_cstr[begin]));
        *inner_len = len;
    } else {
        *ptr = NULL;
        *inner_len = 0;
    }
}

FMBOOL fmstrv_find(const fmstrv_t* strv, const fmstrv_t* token, FMSIZE* out_index) {
    FMBOOL out;
    fmdarray_find(strv, token, &out, out_index);
    return out;
}

FMBOOL fmstrv_find_dv(const fmdstr_t* dstr, const fmstrv_t* token, FMSIZE* out_index) {
    FMBOOL out;
    fmdarray_find(dstr, token, &out, out_index);
    return out;
}

FMBOOL fmdstr_find_vd(const fmstrv_t* strv, const fmdstr_t* token, FMSIZE* out_index) {
    FMBOOL out;
    fmdarray_find(strv, token, &out, out_index);
    return out;
}

char* fmstrv_to_cstr(const fmstrv_t* strv) {
    char* out = FMREALLOC(NULL, strv->len + 1);
    FMSIZE i;
    if (out == NULL) return out;
    for (i = 0; i < strv->len; ++i) out[i] = strv->elems[i];
    out[strv->len] = '\0';
    return out;
}

#endif

/* end of strv - String view type and functions */
