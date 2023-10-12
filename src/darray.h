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
