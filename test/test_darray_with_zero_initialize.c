#include <assert.h>
#include <stdio.h>

#define FMDARRAY_INITAL_CAPACITY 2
#define INCLUDE_FMDARRAY
#include "fmstd.h"

typedef fmdarray_t(int) VecInt;

int main(void) {
    VecInt myvec = {0};
    fmdarray_push(&myvec, -1);
    {
        assert(myvec.len == 1 && "Len wrong!");
        assert(myvec.capacity == FMDARRAY_INITAL_CAPACITY && "Capacity wrong!");
        assert(myvec.elems[0] == -1 && "Value 0 wrong");
        assert(!myvec.error && "In error");
    }
    fmdarray_free(&myvec);
    return 0;
}
