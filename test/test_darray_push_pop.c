#include <assert.h>
#include <stdio.h>

#define FMDARRAY_INITAL_CAPACITY 2
#define INCLUDE_FMDARRAY
#include "fmstd.h"

typedef fmdarray_t(int) VecInt;

int main(void) {
    int value = {0};
    VecInt myvec = {0};
    fmdarray_push(&myvec, -1);
    {
        assert(myvec.len == 1 && "Len wrong!");
        assert(myvec.capacity == FMDARRAY_INITAL_CAPACITY && "Capacity wrong!");
        assert(myvec.elems[0] == -1 && "Value 0 wrong");
        assert(!myvec.error && "In error");
    }

    fmdarray_push(&myvec, -2);
    {
        assert(myvec.len == 2 && "Len wrong!");
        assert(myvec.capacity == FMDARRAY_INITAL_CAPACITY && "Capacity wrong!");
        assert(myvec.elems[0] == -1 && "Value 0 wrong");
        assert(myvec.elems[1] == -2 && "Value 1 wrong");
        assert(!myvec.error && "In error");
    }

    fmdarray_push(&myvec, -3);
    {
        assert(myvec.len == 3 && "Len wrong!");
        assert(myvec.capacity == 4 && "Capacity wrong!");
        assert(myvec.elems[0] == -1 && "Value 0 wrong");
        assert(myvec.elems[1] == -2 && "Value 1 wrong");
        assert(myvec.elems[2] == -3 && "Value 2 wrong");
        assert(!myvec.error && "In error");
    }

    fmdarray_pop(&myvec, &value);
    {
        assert(myvec.len == 2 && "Len wrong!");
        assert(myvec.capacity == 4 && "Capacity wrong!");
        assert(myvec.elems[0] == -1 && "Value 0 wrong");
        assert(myvec.elems[1] == -2 && "Value 1 wrong");
        assert(!myvec.error && "In error");
        assert(value == -3 && "Value out wrong");
    }

    fmdarray_pop(&myvec, &value);
    {
        assert(myvec.len == 1 && "Len wrong!");
        assert(myvec.capacity == 4 && "Capacity wrong!");
        assert(myvec.elems[0] == -1 && "Value 0 wrong");
        assert(value == -2 && "Value out wrong");
        assert(!myvec.error && "In error");
    }

    fmdarray_pop(&myvec, &value);
    {
        assert(myvec.len == 0 && "Len wrong!");
        assert(myvec.capacity == 4 && "Capacity wrong!");
        assert(!myvec.error && "In error");
        assert(value == -1 && "Value out wrong");
    }

    fmdarray_free(&myvec);
    return 0;
}
