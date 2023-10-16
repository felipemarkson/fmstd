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
    FMSIZE req_cap = array_len + initial_len;
    FMSIZE i;
    if ((req_cap > string->capacity))
        fmdarray_set_capacity(string, FMDARRAY_GROWTH_RATE * req_cap);
    if (string->error || (string->elems == NULL)) {
        string->error = FMTRUE;
        return;
    }

    for (i = 0; i < array_len; i++) string->elems[initial_len + i] = array[i];
    string->len = req_cap;
}

fmdstr_t fmdstr_from_cstr(const char null_terminated_cstr[static 1]) {
    FMSIZE len;
    fmdstr_t out = {0};
    const char* temp = null_terminated_cstr;
    if (null_terminated_cstr == NULL) return out;

    len = 0;
    while (*temp != '\0') {
        ++len;
        ++temp;
    }
    if (len == 0) return out;
    fmdstr_push_array(&out, len, null_terminated_cstr);
    return out;
}

char* fmdstr_to_cstr(const fmdstr_t string[static 1]) {
    char* out;
    FMSIZE i;
    out = FMREALLOC(NULL, (string->len + 1) * sizeof(char));
    if (out == NULL) return out;
    for (i = 0; i < string->len; i++) out[i] = string->elems[i];
    out[string->len] = '\0';
    return out;
}

void fmdstr_concat(fmdstr_t first[static 1], const fmdstr_t second[static 1]) {
    fmdstr_push_array(first, second->len, second->elems);
}

void fmdstr_reverse(fmdstr_t dstr[static 1]) { fmdarray_reverse(dstr, char); }

#endif
/* end of dstr - Dynamic string type and functions */
