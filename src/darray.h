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
