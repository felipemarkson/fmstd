#include <assert.h>
#include <stdio.h>
#include <string.h>

#define IMPLEMENT_FMHASHMAP
#include "fmstd.h"

// djb2: http://www.cse.yorku.ca/~oz/hash.html
FMSIZE djb2(const void* key) {
    FMSIZE hash     = 5381;
    const char* ptr = *(char**)key;
    char c;

    while ((c = *(ptr++)))
        hash = ((hash << 5) + hash) + (FMSIZE)c;
    return hash;
}

FMBOOL keycmp(const void* key1, const void* key2) {
    return strcmp(*(char**)key1, *(char**)key2) == 0;
}

int main(void) {
    fmhm_pair_t value = {0};

    fmhm_t hs = {0};
    if (fmhm_init(char*, int, &hs, keycmp, djb2) < 0)
        assert(0 && "Error on init");

    if (fmhm_insert(&hs, &(char*){"FELIPE M DOS S MONTEIRO"}, &(int){1}) < 0)
        assert(0 && "Error on insert");

    if (fmhm_insert(&hs, &(char*){"JOAO JOAO DA SILVA SAURO"}, &(int){-1}) < 0)
        assert(0 && "Error on insert");

    value = fmhm_get(&hs, &(char*){"FELIPE M DOS S MONTEIRO"});
    assert(*(int*)value.ptr_value == 1 && "Error on get");

    value = fmhm_get(&hs, &(char*){"JOAO JOAO DA SILVA SAURO"});
    assert(*(int*)value.ptr_value == -1 && "Error on get");

    fmhm_deinit(&hs);
    return 0;
}
