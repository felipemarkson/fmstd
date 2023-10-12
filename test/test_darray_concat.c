#include <assert.h>
#include <stdio.h>

#define FM_DARRAY_INITAL_CAPACITY 2
#define INCLUDE_FM_DARRAY
#include "fm_std.h"

typedef fm_darray_t(int) VecInt;

int main(void) {
    VecInt a = {0};
    VecInt b = {0};

    fm_darray_push(&a, -1);
    fm_darray_push(&a, -2);
    fm_darray_push(&a, -3);

    fm_darray_push(&b, -4);
    fm_darray_push(&b, -5);
    fm_darray_push(&b, -6);

    fm_darray_concat(&a, &b);
    {
        assert(!a.error && "a in error");
        assert(!b.error && "b in error");

        assert(a.len == 6 && "a len wrong!");
        assert(a.capacity == 6 && "a capacity wrong!");
        assert(b.len == 3 && "b ben wrong!");

        assert(a.elems[0] == -1 && "a[0] wrong");
        assert(a.elems[1] == -2 && "a[1] wrong");
        assert(a.elems[2] == -3 && "a[2] wrong");
        assert(a.elems[3] == -4 && "a[3] wrong");
        assert(a.elems[4] == -5 && "a[4] wrong");
        assert(a.elems[5] == -6 && "a[5] wrong");

        assert(b.elems[0] == -4 && "b[0] wrong");
        assert(b.elems[1] == -5 && "b[0] wrong");
        assert(b.elems[2] == -6 && "b[0] wrong");
    }

    fm_darray_free(&a);
    fm_darray_free(&b);
    return 0;
}
