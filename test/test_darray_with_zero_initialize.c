#include <assert.h>
#include <stdio.h>

#define FM_DARRAY_INITAL_CAPACITY 2
#define INCLUDE_FM_DARRAY
#include "fm_std.h"

typedef fm_darray_t(int) VecInt;

int main(void) {
    VecInt myvec = {0};
    fm_darray_push(&myvec, -1);
    {
        assert(myvec.len == 1 && "Len wrong!");
        assert(myvec.capacity == FM_DARRAY_INITAL_CAPACITY && "Capacity wrong!");
        assert(myvec.elems[0] == -1 && "Value 0 wrong");
        assert(!myvec.error && "In error");
    }
    fm_darray_free(&myvec);
    return 0;
}
