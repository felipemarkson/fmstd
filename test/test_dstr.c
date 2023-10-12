#include <assert.h>
#include <stdio.h>
#include <string.h>

#define IMPLEMENT_FM_DSTR
#include "fm_std.h"

#define A_TEST "abcd"
#define LEN_A sizeof(A_TEST) - 1
#define B_TEST "efghi"
#define LEN_B sizeof(B_TEST) - 1
#define B_REV "ihgfe"
#define C_TEST "1234"
#define LEN_C sizeof(C_TEST) - 1
#define C_REV "4321"

int main(void) {
    fm_dstr_t a = {0};
    fm_dstr_t b = {0};
    fm_dstr_t c = {0};
    char* cstr = {0};
    FM_SIZE index;
    FM_BOOL ret;

    fm_dstr_push_array(&a, LEN_A, A_TEST);
    {
        assert(!a.error && "dstr is in errror");
        cstr = fm_dstr_to_cstr(&a);
        assert(cstr && "fm_dstr_to_cstr returns NULL");
        assert(strcmp(cstr, "abcd") == 0 && "fm_dstr_push_array not match");
        free(cstr);
    }

    b = fm_dstr_from_cstr(B_TEST);
    {
        assert(!b.error && "dstr is in errror");
        cstr = fm_dstr_to_cstr(&b);
        assert(cstr && "fm_dstr_to_cstr returns NULL");
        assert(strcmp(cstr, B_TEST) == 0 && "fm_dstr_push_array not match");
        free(cstr);
    }

    fm_dstr_concat(&a, &b);
    {
        assert(!a.error && "dstr is in errror");
        cstr = fm_dstr_to_cstr(&a);
        assert(cstr && "fm_dstr_to_cstr returns NULL");
        assert(strcmp(cstr, A_TEST B_TEST) == 0 && "fm_dstr_concat not match");
        free(cstr);
    }

    fm_dstr_push_array(&a, LEN_A, A_TEST);
    assert(!a.error && "dstr is in errror");
    ret = fm_dstr_find(&a, &b, &index);
    {
        assert(ret == FM_TRUE && "fm_dstr_find not found");
        assert(index == LEN_A && "fm_dstr_find wrong index");
    }

    fm_dstr_reverse(&b);
    {
        cstr = fm_dstr_to_cstr(&b);
        assert(cstr && "fm_dstr_to_cstr returns NULL");
        assert(strcmp(cstr, B_REV) == 0 && "fm_dstr_reverse not match");
        free(cstr);
    }

    fm_dstr_push_array(&c, LEN_C, C_TEST);
    fm_dstr_reverse(&c);
    {
        assert(!c.error && "dstr is in errror");
        cstr = fm_dstr_to_cstr(&c);
        assert(cstr && "fm_dstr_to_cstr returns NULL");
        assert(strcmp(cstr, C_REV) == 0 && "fm_dstr_reverse not match");
        free(cstr);
    }

    assert((!fm_dstr_eq(&a, &b)) && "fm_dstr_eq returns true but must be false");
    assert((!fm_dstr_eq(&c, &b)) && "fm_dstr_eq returns true but must be false");
    assert((fm_dstr_eq(&a, &a)) && "fm_dstr_eq returns false but must be true");
    assert((fm_dstr_eq(&b, &b)) && "fm_dstr_eq returns false but must be true");
    assert((fm_dstr_eq(&c, &c)) && "fm_dstr_eq returns false but must be true");

    fm_darray_free(&a);
    fm_darray_free(&b);
    fm_darray_free(&c);
    return 0;
}
