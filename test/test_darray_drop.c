#include <assert.h>
#include <stdio.h>

#define FMDARRAY_INITAL_CAPACITY 2
#define INCLUDE_FMDARRAY
#include "fmstd.h"

typedef fmdarray_t(int) VecInt;

int main(void) {
    int value    = {0};
    VecInt myvec = {0};
    fmdarray_push(&myvec, -1);
    fmdarray_push(&myvec, -2);
    fmdarray_push(&myvec, -3);
    fmdarray_push(&myvec, -4);
    fmdarray_push(&myvec, -5);

    fmdarray_drop(&myvec, 2, &value);
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

    fmdarray_free(&myvec);
    return 0;
}
