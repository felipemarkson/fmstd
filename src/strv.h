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
fmdstr_t fmstrv_to_dstr(const fmstrv_t* strv);

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
    const char* temp = null_terminated_cstr;
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

FMBOOL fmdstr_find_av(const char* array, FMSIZE size, const fmstrv_t* token,
                      FMSIZE* out_index) {
    fmstrv_t temp = {0};
    FMBOOL out;
    fmstrv_from_array(array, size, &temp, 0, size);
    fmdarray_find(&temp, token, &out, out_index);
    return out;
}

FMBOOL fmdstr_find_va(const fmstrv_t* strv, const char* array_token, FMSIZE size,
                      FMSIZE* out_index) {
    fmstrv_t temp = {0};
    FMBOOL out;

    fmstrv_from_array(array_token, size, &temp, 0, size);
    fmdarray_find(strv, &temp, &out, out_index);
    return out;
}

FMBOOL fmdstr_find_cv(const char* null_terminated_cstr, const fmstrv_t* token,
                      FMSIZE* out_index) {
    fmstrv_t temp = {0};
    FMBOOL out;
    const char* tempptr = null_terminated_cstr;
    FMSIZE size = 0;
    while (*tempptr != '\0') {
        ++size;
        ++tempptr;
    }
    fmstrv_from_array(null_terminated_cstr, size, &temp, 0, size);
    fmdarray_find(&temp, token, &out, out_index);
    return out;
}

FMBOOL fmdstr_find_vc(const fmstrv_t* strv, const char* null_terminated_token,
                      FMSIZE* out_index) {
    fmstrv_t temp = {0};
    FMBOOL out;
    const char* tempptr = null_terminated_token;
    FMSIZE size = 0;
    while (*tempptr != '\0') {
        ++size;
        ++tempptr;
    }
    fmstrv_from_array(null_terminated_token, size, &temp, 0, size);
    fmdarray_find(strv, &temp, &out, out_index);
    return out;
}

char* fmstrv_to_cstr(const fmstrv_t* strv) {
    char* out = FMREALLOC(NULL, (strv->len + 1) * sizeof(char));
    FMSIZE i;
    if (out == NULL) return out;
    for (i = 0; i < strv->len; ++i) out[i] = strv->elems[i];
    out[strv->len] = '\0';
    return out;
}

fmdstr_t fmstrv_to_dstr(const fmstrv_t* strv) {
    fmdstr_t out = {0};
    FMSIZE capacity = 2 * (strv->len) * sizeof(char);
    FMSIZE i;
    out.elems = FMREALLOC(NULL, capacity);
    if (out.elems == NULL) {
        out.error = FMTRUE;
        return out;
    }
    for (i = 0; i < strv->len; ++i) out.elems[i] = strv->elems[i];
    out.capacity = capacity;
    out.len = strv->len;
    return out;
}

#endif

/* end of strv - String view type and functions */
