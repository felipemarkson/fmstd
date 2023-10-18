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
