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
