#include <assert.h>
#include <stdio.h>

#define FM_DARRAY_INITAL_CAPACITY 2
#define INCLUDE_FM_DARRAY
#include "fm_std.h"

typedef fm_darray_t(int) VecInt;

int main(void) {
    int value = {0};
    VecInt myvec = {0};
    fm_darray_push(&myvec, -1);
    fm_darray_push(&myvec, -2);
    fm_darray_push(&myvec, -3);
    fm_darray_push(&myvec, -4);
    fm_darray_push(&myvec, -5);

    fm_darray_drop(&myvec, 2, &value);
    {
        assert(!myvec.error);
        assert(myvec.len == 4 && "Len wrong!");
        assert(myvec.elems[0] == -1 && "Value 0 wrong");
        assert(myvec.elems[1] == -2 && "Value 1 wrong");
        assert(myvec.elems[2] == -4 && "Value 2 wrong");
        assert(myvec.elems[3] == -5 && "Value 3 wrong");
        assert(value == -3 && "Value dropped wrong");
        assert(!myvec.error && "In error");
    }

    fm_darray_free(&myvec);
    return 0;
}
