#include <assert.h>
#include <stdio.h>

#define INCLUDE_FMDARRAY
#include "fmstd.h"

typedef fmdarray_t(int) VecInt;

int main(void) {
    {
        VecInt pushing = {0};
        fmdarray_push(&pushing, -1);
        fmdarray_push(&pushing, -2);
        fmdarray_push(&pushing, -3);
        fmdarray_push(&pushing, -4);
        fmdarray_push(&pushing, -5);

        fmdarray_insert(&pushing, 2, 0);
        {
            assert(!pushing.error && "In error");
            assert(pushing.len == 6 && "Len wrong!");
            assert(pushing.elems[0] == -1 && "Value 0 wrong");
            assert(pushing.elems[1] == -2 && "Value 1 wrong");
            assert(pushing.elems[2] == 0 && "Value 2 wrong");
            assert(pushing.elems[3] == -3 && "Value 3 wrong");
            assert(pushing.elems[4] == -4 && "Value 4 wrong");
            assert(pushing.elems[5] == -5 && "Value 5 wrong");
        }
        fmdarray_free(&pushing);
    }
    {
        VecInt preallocated = {0};
        fmdarray_set_capacity(&preallocated, 5);
        assert(!preallocated.error && "In error");
        fmdarray_push(&preallocated, -1);
        assert(!preallocated.error && "In error");
        fmdarray_push(&preallocated, -2);
        assert(!preallocated.error && "In error");
        fmdarray_push(&preallocated, -3);
        assert(!preallocated.error && "In error");
        fmdarray_push(&preallocated, -4);
        assert(!preallocated.error && "In error");

        fmdarray_insert(&preallocated, 2, 0);
        {
            assert(!preallocated.error && "In error");
            assert(preallocated.len == 5 && "Len wrong!");
            assert(preallocated.elems[0] == -1 && "Value 0 wrong");
            assert(preallocated.elems[1] == -2 && "Value 1 wrong");
            assert(preallocated.elems[2] == 0 && "Value 2 wrong");
            assert(preallocated.elems[3] == -3 && "Value 3 wrong");
            assert(preallocated.elems[4] == -4 && "Value 4 wrong");
        }
        fmdarray_free(&preallocated);
    }

    {
        VecInt needallocation = {0};
        fmdarray_set_capacity(&needallocation, 4);
        assert(!needallocation.error && "In error");
        fmdarray_push(&needallocation, -1);
        assert(!needallocation.error && "In error");
        fmdarray_push(&needallocation, -2);
        assert(!needallocation.error && "In error");
        fmdarray_push(&needallocation, -3);
        assert(!needallocation.error && "In error");
        fmdarray_push(&needallocation, -4);
        assert(!needallocation.error && "In error");

        fmdarray_insert(&needallocation, 2, 0);
        {
            assert(!needallocation.error && "In error");
            assert(needallocation.len == 5 && "Len wrong!");
            assert(needallocation.elems[0] == -1 && "Value 0 wrong");
            assert(needallocation.elems[1] == -2 && "Value 1 wrong");
            assert(needallocation.elems[2] == 0 && "Value 2 wrong");
            assert(needallocation.elems[3] == -3 && "Value 3 wrong");
            assert(needallocation.elems[4] == -4 && "Value 4 wrong");
        }
        fmdarray_free(&needallocation);
    }
    return 0;
}
