/*
Automaticaly created in 2023-10-12 21:52:08+00:00
fm_std.h - Felipe Monteiro's C standard library

Check the [repostiory](https://github.com/felipemarkson/fm_std) for more details.
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
#ifdef IMPLEMENT_FM_DSTR
#define INCLUDE_FM_DSTR
#endif

#ifdef INCLUDE_FM_DSTR
#define INCLUDE_FM_DARRAY
#endif

#ifdef INCLUDE_FM_DARRAY
#define INCLUDE_FM_COMMON
#endif
/* ends of dependency - Dependecy tree definitions */
/* common - Common definitions used in all library */
#ifdef INCLUDE_FM_COMMON
#ifndef FM_COMMON_H

#ifndef NULL
#include <stddef.h>
#ifndef NULL
#error "Need to define NULL"
#endif
#endif

#ifndef FM_REALLOC
#include <stdlib.h>
#define FM_REALLOC realloc
#endif

#ifndef FM_FREE
#include <stdlib.h>
#define FM_FREE free
#endif

#ifndef FM_SIZE
#include <stddef.h>
#define FM_SIZE size_t
#endif

#ifndef FM_BOOL
#define FM_BOOL unsigned int
#endif

#ifndef FM_TRUE
#define FM_TRUE ((FM_BOOL)1)
#endif

#ifndef FM_FALSE
#define FM_FALSE ((FM_BOOL)0)
#endif

#endif
#endif
/* ends of common - Common definitions used in all library */
/* darray - Generic Dynamic Array macros */
#ifdef INCLUDE_FM_DARRAY

#ifndef FM_DARRAY_INCLUDED
#define FM_DARRAY_INCLUDED

#ifndef FM_DARRAY_INITAL_CAPACITY
#define FM_DARRAY_INITAL_CAPACITY 8
#endif
#ifndef FM_DARRAY_GROWTH_RATE
#define FM_DARRAY_GROWTH_RATE 2
#endif

#define fm_darray_t(type) \
    struct {              \
        type* elems;      \
        FM_SIZE len;      \
        FM_SIZE capacity; \
        FM_BOOL error;    \
    }

#define fm_darray_set_capacity(ptr_darray, new_capacity)                            \
    do {                                                                            \
        void* new_elems;                                                            \
        if ((ptr_darray)->capacity == (new_capacity)) break;                        \
        new_elems = FM_REALLOC((ptr_darray)->elems,                                 \
                               (new_capacity) * sizeof(*((ptr_darray)->elems)));    \
        if (new_elems == NULL) {                                                    \
            (ptr_darray)->error = FM_TRUE;                                          \
            break;                                                                  \
        }                                                                           \
        (ptr_darray)->elems = new_elems;                                            \
        (ptr_darray)->capacity = (new_capacity);                                    \
        if ((ptr_darray)->len > (new_capacity)) (ptr_darray)->len = (new_capacity); \
    } while (0)

#define fm_darray_free(ptr_darray) \
    do {                              \
        FM_FREE((ptr_darray)->elems); \
        (ptr_darray)->elems = NULL;   \
        (ptr_darray)->len = 0;        \
        (ptr_darray)->capacity = 0;   \
    } while (0)

#define fm_darray_push(ptr_darray, new_elem)                                   \
    do {                                                                       \
        FM_SIZE len = (ptr_darray)->len;                                       \
        FM_SIZE cap = (ptr_darray)->capacity;                                  \
        if (len < cap) {                                                       \
            (ptr_darray)->elems[len] = new_elem;                               \
            ++(ptr_darray)->len;                                               \
        } else {                                                               \
            FM_SIZE new_cap =                                                  \
                cap ? FM_DARRAY_GROWTH_RATE * cap : FM_DARRAY_INITAL_CAPACITY; \
            fm_darray_set_capacity(ptr_darray, new_cap);                       \
            if ((ptr_darray)->error) break;                                    \
            (ptr_darray)->elems[len] = new_elem;                               \
            ++(ptr_darray)->len;                                               \
        }                                                                      \
    } while (0)

#define fm_darray_pop(ptr_darray, out_ptr_elem)                                     \
    do {                                                                            \
        FM_SIZE len = (ptr_darray)->len;                                            \
        FM_SIZE cap = (ptr_darray)->capacity;                                       \
        if (len == 0 || cap == 0) {                                                 \
            (ptr_darray)->error = FM_TRUE;                                          \
        } else {                                                                    \
            if (out_ptr_elem != NULL) *out_ptr_elem = (ptr_darray)->elems[len - 1]; \
            --(ptr_darray)->len;                                                    \
        }                                                                           \
    } while (0)

#define fm_darray_drop(ptr_darray, index, out_ptr_elem)                       \
    do {                                                                      \
        FM_SIZE len = (ptr_darray)->len;                                      \
        FM_SIZE i = index;                                                    \
        FM_SIZE j;                                                            \
        if (i >= len) {                                                       \
            (ptr_darray)->error = FM_TRUE;                                    \
        } else {                                                              \
            if (out_ptr_elem != NULL) *out_ptr_elem = (ptr_darray)->elems[i]; \
            for (j = i + 1; j < len; j++)                                     \
                (ptr_darray)->elems[j - 1] = (ptr_darray)->elems[j];          \
            --(ptr_darray)->len;                                              \
        }                                                                     \
    } while (0)

#define fm_darray_insert(ptr_darray, index, out_ptr_elem)                     \
    do {                                                                      \
        FM_SIZE len = (ptr_darray)->len;                                      \
        FM_SIZE i = index;                                                    \
        FM_SIZE j;                                                            \
        if (i >= len) {                                                       \
            (ptr_darray)->error = FM_TRUE;                                    \
        } else {                                                              \
            if (out_ptr_elem != NULL) *out_ptr_elem = (ptr_darray)->elems[i]; \
            for (j = i + 1; j < len; j++)                                     \
                (ptr_darray)->elems[j - 1] = (ptr_darray)->elems[j];          \
            --(ptr_darray)->len;                                              \
        }                                                                     \
    } while (0)

#define fm_darray_concat(first_ptr_darray, sec_ptr_darray)                           \
    do {                                                                             \
        FM_SIZE index;                                                               \
        FM_SIZE req_cap = (first_ptr_darray)->len + (sec_ptr_darray)->len;           \
        FM_SIZE actual_cap = (first_ptr_darray)->capacity;                           \
        if (req_cap > actual_cap) fm_darray_set_capacity(first_ptr_darray, req_cap); \
        if ((first_ptr_darray)->error) break;                                        \
        for (index = (first_ptr_darray)->len; index < req_cap; index++)              \
            (first_ptr_darray)->elems[index] =                                       \
                (sec_ptr_darray)->elems[index - (first_ptr_darray)->len];            \
        (first_ptr_darray)->len = req_cap;                                           \
    } while (0)

#define fm_darray_reverse(first_ptr_darray, type)                        \
    do {                                                                 \
        type temp;                                                       \
        FM_SIZE i, j;                                                    \
        j = ((first_ptr_darray)->len - 1);                               \
        for (i = 0; i < ((first_ptr_darray)->len / 2); i++) {            \
            temp = (first_ptr_darray)->elems[i];                         \
            (first_ptr_darray)->elems[i] = (first_ptr_darray)->elems[j]; \
            (first_ptr_darray)->elems[j] = temp;                         \
            --j;                                                         \
        }                                                                \
    } while (0)

#define fm_darray_eq(first_ptr_darray, second_ptr_darray, out_ptr_bool)        \
    do {                                                                       \
        FM_SIZE i;                                                             \
        *(out_ptr_bool) = FM_FALSE;                                            \
        if ((first_ptr_darray)->len != (second_ptr_darray)->len) {             \
            break;                                                             \
        }                                                                      \
        for (i = 0; i < (first_ptr_darray)->len; ++i) {                        \
            if ((first_ptr_darray)->elems[i] == (second_ptr_darray)->elems[i]) \
                *(out_ptr_bool) = FM_TRUE;                                     \
            else {                                                             \
                *(out_ptr_bool) = FM_FALSE;                                    \
                break;                                                         \
            }                                                                  \
        }                                                                      \
    } while (0)

#endif
#endif
/* ends of darray - Generic Dynamic Array macros */
/* dstr - Dynamic string type and functions */
#ifdef INCLUDE_FM_DSTR

#ifndef FM_DSTR_INCLUDED
#define FM_DSTR_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/*
### Members:

- `char* elems`      : (`?`) Allocated array;
- `FM_SIZE len`      : Number of the elements in the array (lenght);
- `FM_SIZE capacity` : Array's capacity to store elements without reallocation;
- `FM_BOOL error`    : Indicates if the array is in error state.

### Memory Managment:

Use `FM_FREE` in `elems` to deallocate the memory. You can also use
`darray.h/fm_darray_free` to have a nicer deallocation.

`NOTE:` If the elements are also dynamic-allocated, the `FM_FREE` and
`darray.h/fm_darray_free` will NOT deallocate these elements.

`NOTE:` Set the `len` to zero to zeroing the dynamic string (not deallocate).

`NOTE:` Zero-initilized `fm_dstr_t` are valid.
*/
typedef fm_darray_t(char) fm_dstr_t;

/* Creates a new dynamic string by copying an `'\0'`-terminated array of `char` */
fm_dstr_t fm_dstr_from_cstr(const char null_terminated_cstr[]);

/* Push (by copying) an array of `char` into the dynamic string */
void fm_dstr_push_array(fm_dstr_t* string, FM_SIZE array_len, const char array[]);

/*
It allocates a new `'\0'`-terminated array of `char` and copy dynamic string's
characters into it. The user is reponsible to free the returned array
*/
char* fm_dstr_to_cstr(const fm_dstr_t* string);

/* Push (by copying) the second dynamic string into the first one */
void fm_dstr_concat(fm_dstr_t* first, const fm_dstr_t* second);

/* Dynamic string equality comparision. Checks the lenght and the elements.*/
FM_BOOL fm_dstr_eq(const fm_dstr_t* dstr, const fm_dstr_t* other);

/*
Check if the `token` is in `dstr` storing the location in `index`.
Returns `FM_TRUE` if found, `FM_FALSE`otherwise
*/
FM_BOOL fm_dstr_find(const fm_dstr_t* dstr, const fm_dstr_t* token, FM_SIZE* index);

/* Reverts the dynamic string */
void fm_dstr_reverse(fm_dstr_t* dstr);

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
#ifdef IMPLEMENT_FM_DSTR

void fm_dstr_push_array(fm_dstr_t* string, FM_SIZE array_len, const char array[]) {
    FM_SIZE req_cap = array_len + string->len;
    FM_SIZE i;
    if ((req_cap > string->capacity) || (string->elems == NULL))
        fm_darray_set_capacity(string, FM_DARRAY_GROWTH_RATE * req_cap);
    if (string->error) return;

    for (i = 0; i < array_len; i++) string->elems[string->len + i] = array[i];
    string->len = req_cap;
}

fm_dstr_t fm_dstr_from_cstr(const char null_terminated_cstr[]) {
    FM_SIZE len;
    fm_dstr_t out = {0};
    const char* temp;
    if (null_terminated_cstr == NULL) return out;
    temp = null_terminated_cstr;
    len = 0;
    while (*temp != '\0') {
        ++len;
        ++temp;
    }
    if (len == 0) return out;
    fm_dstr_push_array(&out, len, null_terminated_cstr);
    return out;
}

char* fm_dstr_to_cstr(const fm_dstr_t* string) {
    char* out;
    FM_SIZE i;
    out = FM_REALLOC(NULL, (string->len + 1) * sizeof(char));
    if (out == NULL) return out;
    for (i = 0; i < string->len; i++) out[i] = string->elems[i];
    out[string->len] = '\0';
    return out;
}

void fm_dstr_concat(fm_dstr_t* first, const fm_dstr_t* second) {
    fm_dstr_push_array(first, second->len, second->elems);
}

FM_BOOL fm_dstr_find(const fm_dstr_t* dstr, const fm_dstr_t* token, FM_SIZE* index) {
    FM_SIZE i, j;
    for (i = 0; i < dstr->len; i++) {
        FM_BOOL found;
        FM_SIZE k;
        k = i;
        found = FM_TRUE;
        for (j = 0; j < token->len; j++) {
            if ((k < dstr->len) && (dstr->elems[k] == token->elems[j])) {
                ++k;
            } else {
                found = FM_FALSE;
                break;
            }
        }
        if (found) {
            *index = i;
            return found;
        }
    }
    return FM_FALSE;
}

void fm_dstr_reverse(fm_dstr_t* dstr) { fm_darray_reverse(dstr, char); }

FM_BOOL fm_dstr_eq(const fm_dstr_t* dstr, const fm_dstr_t* other) {
    FM_BOOL out;
    fm_darray_eq(dstr, other, &out);
    return out;
}
#endif
/* end of dstr - Dynamic string type and functions */
