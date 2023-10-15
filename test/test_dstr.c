#include <assert.h>
#include <stdio.h>
#include <string.h>

#define IMPLEMENT_FMDSTR
#include "fmstd.h"

#define A_TEST "abcd"
#define LEN_A sizeof(A_TEST) - 1
#define B_TEST "efghi"
#define LEN_B sizeof(B_TEST) - 1
#define B_REV "ihgfe"
#define C_TEST "1234"
#define LEN_C sizeof(C_TEST) - 1
#define C_REV "4321"

int main(void) {
    fmdstr_t a = {0};
    fmdstr_t b = {0};
    fmdstr_t c = {0};
    fmdstr_t d = {0};
    char* cstr = {0};
    FMSIZE index;
    FMBOOL ret;

    fmdstr_push_array(&a, LEN_A, A_TEST);
    {
        assert(!a.error && "dstr is in errror");
        cstr = fmdstr_to_cstr(&a);
        assert(cstr && "fmdstr_to_cstr returns NULL");
        assert(strcmp(cstr, "abcd") == 0 && "fmdstr_push_array not match");
        free(cstr);
    }

    b = fmdstr_from_cstr(B_TEST);
    {
        assert(!b.error && "dstr is in errror");
        cstr = fmdstr_to_cstr(&b);
        assert(cstr && "fmdstr_to_cstr returns NULL");
        assert(strcmp(cstr, B_TEST) == 0 && "fmdstr_push_array not match");
        free(cstr);
    }

    fmdstr_push_array(&c, LEN_C, C_TEST);
    fmdstr_concat(&a, &b);
    fmdstr_concat(&a, &c);
    {
        assert(!a.error && "dstr is in errror");
        cstr = fmdstr_to_cstr(&a);
        assert(cstr && "fmdstr_to_cstr returns NULL");
        assert(strcmp(cstr, A_TEST B_TEST C_TEST) == 0 && "fmdstr_concat not match");
        free(cstr);
    }

    fmdstr_push_array(&a, LEN_A, A_TEST);
    ret = fmdstr_find(&a, &b, &index);
    {
        assert(ret == FMTRUE && "fmdstr_find not found");
        assert(index == LEN_A && "fmdstr_find wrong index");
    }
    ret = fmdstr_find(&a, &c, &index);
    {
        assert(ret == FMTRUE && "fmdstr_find not found");
        assert(index == (LEN_A + LEN_B) && "fmdstr_find wrong index");
    }
    d = fmdstr_from_cstr("NOTFOUND");
    ret = fmdstr_find(&a, &d, &index);
    {
        assert(ret == FMFALSE && "fmdstr_find should not found");
    }
    ret = fmdstr_find_da(&a, B_TEST, LEN_B, &index);
    {
        assert(ret == FMTRUE && "fmdstr_find not found");
        assert(index == LEN_A && "fmdstr_find wrong index");
    }
    ret = fmdstr_find_dc(&a, B_TEST, &index);
    {
        assert(ret == FMTRUE && "fmdstr_find not found");
        assert(index == LEN_A && "fmdstr_find wrong index");
    }


    fmdstr_reverse(&b);
    {
        cstr = fmdstr_to_cstr(&b);
        assert(cstr && "fmdstr_to_cstr returns NULL");
        assert(strcmp(cstr, B_REV) == 0 && "fmdstr_reverse not match");
        free(cstr);
    }

    fmdstr_reverse(&c);
    {
        assert(!c.error && "dstr is in errror");
        cstr = fmdstr_to_cstr(&c);
        assert(cstr && "fmdstr_to_cstr returns NULL");
        assert(strcmp(cstr, C_REV) == 0 && "fmdstr_reverse not match");
        free(cstr);
    }

    assert((!fmdstr_eq(&a, &b)) && "fmdstr_eq returns true but must be false");
    assert((!fmdstr_eq(&c, &b)) && "fmdstr_eq returns true but must be false");
    assert((fmdstr_eq(&a, &a)) && "fmdstr_eq returns false but must be true");
    assert((fmdstr_eq(&b, &b)) && "fmdstr_eq returns false but must be true");
    assert((fmdstr_eq(&c, &c)) && "fmdstr_eq returns false but must be true");

    fmdarray_free(&a);
    fmdarray_free(&b);
    fmdarray_free(&c);
    fmdarray_free(&d);
    return 0;
}
