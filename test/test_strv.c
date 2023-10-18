#include <assert.h>
#include <stdio.h>
#include <string.h>

#define IMPLEMENT_FMSTRV
#define IMPLEMENT_FMDSTR
#include "fmstd.h"

int main(void) {
    {
        fmdstr_t testdstr = fmdstr_from_cstr("This is a message to test!");
        fmstrv_t strview  = {0};
        char* cstr        = {0};
        fmstrv_from_dstr(&testdstr, &strview, 10, 7);
        cstr = fmstrv_to_cstr(&strview);
        assert(cstr && "fmstrv_to_cstr return null");
        assert(strcmp("message", cstr) == 0 && "String View is not correct");
        FMFREE(cstr);
        fmdarray_free(&testdstr);
    }

    {
        fmstrv_t strview = {0};
        char* cstr       = {0};
        fmstrv_from_cstr("This is a message to test!", &strview, 21, 5);
        cstr = fmstrv_to_cstr(&strview);
        assert(cstr && "fmstrv_to_cstr return null");
        assert(strcmp("test!", cstr) == 0 && "String View is not correct");
        FMFREE(cstr);
    }

    {
        fmstrv_t strview = {0};
        char* cstr       = {0};
        fmstrv_from_array(26, "This is a message to test!", &strview, 4, 6);
        cstr = fmstrv_to_cstr(&strview);
        assert(cstr && "fmstrv_to_cstr return null");
        assert(strcmp(" is a ", cstr) == 0 && "String View is not correct");
        FMFREE(cstr);
    }

    {
        fmstrv_t strview = {0};
        fmstrv_t other   = {0};
        fmstrv_from_array(26, "This is a message to test!", &strview, 4, 6);
        fmstrv_from_array(26, "This is a different message to test!", &other, 4, 6);
        assert(fmstrv_eq(&strview, &other) && "fmstrv_eq return false");
    }

    {
        fmstrv_t strview = {0};
        fmstrv_t other   = {0};
        fmstrv_from_array(26, "This is a message to test!", &strview, 4, 6);
        fmstrv_from_array(26, "This is a different message to test!", &other, 4, 6);
        assert(fmstrv_eq(&strview, &other) && "fmstrv_eq return false");
    }
    {
        fmstrv_t strview = {0};
        fmstrv_t other   = {0};
        fmstrv_from_array(26, "This is a message to test!", &strview, 4, 6);
        fmstrv_from_array(26, "This is a different message to test!", &other, 4, 7);
        assert(!fmstrv_eq(&strview, &other) && "fmstrv_eq return false");
    }
    {
        fmstrv_t strview = {0};
        fmstrv_t other   = {0};
        fmstrv_from_array(26, "This is a message to test!", &strview, 4, 7);
        fmstrv_from_array(26, "This is a different message to test!", &other, 4, 7);
        assert(!fmstrv_eq(&strview, &other) && "fmstrv_eq return false");
    }

    return 0;
}
