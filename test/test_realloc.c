#include <assert.h>

#define FM_REALLOC my_realloc
#define INCLUDE_FM_DARRAY
#include "fm_std.h"

typedef fm_darray_t(int) VecInt;

void* my_realloc(void* ptr, size_t size) {
    (void)ptr;
    (void)size;
    return NULL;
}
int main(void) {
    VecInt vec = {0};
    fm_darray_push(&vec, 1);
    assert(vec.error && "fm_darray_push with my_realloc should set error to true");
    return 0;
}
