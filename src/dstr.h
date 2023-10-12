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
