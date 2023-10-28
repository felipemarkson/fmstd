/* fmhm_t */

// TODO: TESTING ON BIGENDIAN
#ifdef INCLUDE_FMHASHMAP

#ifndef FMHASHMAP_INCLUDED
#define FMHASHMAP_INCLUDED

typedef FMBOOL (*fmcompkey_f)(const void* ptr_key1, const void* ptr_key2);
typedef FMSIZE (*fmhash_f)(const void* ptr_key);

typedef struct {
    unsigned char* data;
    FMSIZE capacity;
    fmcompkey_f eqkeyfunc;
    fmhash_f hashfunc;
    FMSIZE size_key;
    FMSIZE size_value;
    FMSIZE size_data;
    FMSIZE iter_index;
} fmhm_t;

typedef struct {
    const void* ptr_key;
    const void* ptr_value;
} fmhm_pair_t;

int fmhm_init_inner(FMSIZE size_key, FMSIZE size_value, fmhm_t hs[static 1],
                    fmcompkey_f eqkeyfunc, fmhash_f hashfunc);

void fmhm_deinit(fmhm_t hs[static 1]);

#define fmhm_init(K, V, hsptr, eqkeyfunc, hashfunc)                                    \
    fmhm_init_inner(sizeof(K), sizeof(V), (hsptr), (eqkeyfunc), (hashfunc))

fmhm_pair_t fmhm_get(fmhm_t hs[static 1], const void* ptr_key);
int fmhm_insert(fmhm_t hs[static 1], const void* ptr_key, const void* ptr_value);

#endif

#ifdef IMPLEMENT_FMHASHMAP
#define FMHASHMAP_INIT_CAP 2U

void* fmhm_ptr_key_by_index(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    return &(hs->data[data_index]);
}

void fmhm_insert_key(fmhm_t hs[static 1], FMSIZE index, const void* ptr_key) {
    unsigned char* dest      = fmhm_ptr_key_by_index(hs, index);
    const unsigned char* src = ptr_key;
    for (FMSIZE i = 0; i < hs->size_key; i++)
        dest[i] = src[i];
}

void* fmhm_ptr_value_by_index(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    return &(hs->data[data_index]) + hs->size_key;
}

void fmhm_insert_value(fmhm_t hs[static 1], FMSIZE index, const void* ptr_value) {
    unsigned char* dest      = fmhm_ptr_value_by_index(hs, index);
    const unsigned char* src = ptr_value;
    for (FMSIZE i = 0; i < hs->size_value; i++)
        dest[i] = src[i];
}

FMBOOL fmhm_istaken(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    return *(&(hs->data[data_index]) + hs->size_key + hs->size_value);
}

void fmhm_set_taken(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    *(&(hs->data[data_index]) + hs->size_key + hs->size_value) = 1;
}
void fmhm_unset_taken(fmhm_t hs[static 1], FMSIZE index) {
    FMSIZE data_index = index * (hs->size_key + hs->size_value + 1U);
    *(&(hs->data[data_index]) + hs->size_key + hs->size_value) = 0;
}

int fmhm_init_inner(FMSIZE size_key, FMSIZE size_value, fmhm_t hs[static 1],
                    fmcompkey_f eqkeyfunc, fmhash_f hashfunc) {

    FMSIZE size_data    = FMHASHMAP_INIT_CAP * (size_key + size_value + 1U);
    unsigned char* data = FMREALLOC(NULL, size_data);
    if (data == NULL) {
        FMFREE(data);
        return -1;
    }
    for (FMSIZE i = 0; i < size_data; i++)
        data[i] = 0;

    hs->data       = data;
    hs->capacity   = FMHASHMAP_INIT_CAP;
    hs->eqkeyfunc  = eqkeyfunc;
    hs->hashfunc   = hashfunc;
    hs->size_key   = size_key;
    hs->size_value = size_value;
    hs->size_data  = size_data;
    hs->iter_index = 0;
    return 0;
}

fmhm_pair_t fmhm_get(fmhm_t hs[static 1], const void* ptr_key) {
    FMSIZE i     = hs->hashfunc(ptr_key) % hs->capacity;
    FMSIZE count = 0;
    while (count < hs->capacity) {
        while (!fmhm_istaken(hs, i)) {
            ++i;
            ++count;
            if (count >= hs->capacity)
                return (fmhm_pair_t){NULL, NULL};
            if (i >= hs->capacity)
                i = 0;
        }

        const void* ptr_otherkey = fmhm_ptr_key_by_index(hs, i);
        if (hs->eqkeyfunc(ptr_otherkey, ptr_key))
            return (fmhm_pair_t){ptr_otherkey, fmhm_ptr_value_by_index(hs, i)};
        ++i;
        ++count;
        if (count >= hs->capacity)
            return (fmhm_pair_t){NULL, NULL};
        if (i >= hs->capacity)
            i = 0;
    }
    return (fmhm_pair_t){NULL, NULL};
}

int fmhm_insert(fmhm_t hs[static 1], const void* ptr_key, const void* ptr_value) {
    if (hs->capacity == 0U)
        return -1;

    FMSIZE i     = hs->hashfunc(ptr_key) % hs->capacity;
    FMSIZE count = 0;

    while (fmhm_istaken(hs, i)) {
        ++i;
        ++count;
        if (count >= hs->capacity)
            return -1;
        if (i >= hs->capacity)
            i = 0;
    }
    fmhm_insert_key(hs, i, ptr_key);
    fmhm_insert_value(hs, i, ptr_value);
    fmhm_set_taken(hs, i);
    hs->iter_index = 0;
    return 0;
}

void fmhm_deinit(fmhm_t hs[static 1]) {
    FMFREE(hs->data);
    hs->data     = NULL;
    hs->capacity = 0;
}

void fmhm_iter_start(fmhm_t hs[static 1]) { hs->iter_index = 0; }

int fmhm_iter_next(fmhm_t hs[static 1], fmhm_pair_t out[static 1]) {
    while (hs->iter_index < hs->capacity) {
        if (fmhm_istaken(hs, hs->iter_index)) {
            out->ptr_key   = fmhm_ptr_key_by_index(hs, hs->iter_index);
            out->ptr_value = fmhm_ptr_value_by_index(hs, hs->iter_index);
            hs->iter_index++;
            return 0;
        }
        hs->iter_index++;
    }
    return -1;
}

#endif
#endif
/* end of fmhm_t*/
