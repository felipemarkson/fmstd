#include <assert.h>

#define FMREALLOC my_realloc
#define INCLUDE_FMDARRAY
#include "fmstd.h"

typedef fmdarray_t(int) VecInt;

void* my_realloc(void* ptr, size_t size) {
    (void)ptr;
    (void)size;
    return NULL;
}
int main(void) {
    VecInt vec = {0};
    fmdarray_push(&vec, 1);
    assert(vec.error && "fmdarray_push with my_realloc should set error to true");
    return 0;
}
