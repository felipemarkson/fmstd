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
