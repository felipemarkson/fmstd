#include <assert.h>
#include <stdio.h>
#include <string.h>

#define IMPLEMENT_FMSTRV
#define IMPLEMENT_FMDSTR
#include "fmstd.h"

int main(void){    
    {
        fmdstr_t testdstr = fmdstr_from_cstr("This is a message to test!");
        fmstrv_t strview = {0};
        char* cstr = {0};
        fmstrv_from_dstr(&testdstr, &strview, 10, 7);
        cstr = fmstrv_to_cstr(&strview);
        assert(strcmp("message", cstr) == 0 && "String View is not correct");
        FMFREE(cstr);
        fmdarray_free(&testdstr);
    }

    {
        fmstrv_t strview = {0};
        char* cstr = {0};
        fmstrv_from_cstr("This is a message to test!", &strview, 21, 5);
        cstr = fmstrv_to_cstr(&strview);
        assert(strcmp("test!", cstr) == 0 && "String View is not correct");
        FMFREE(cstr);
    }

    {
        fmstrv_t strview = {0};
        char* cstr = {0};
        fmstrv_from_array("This is a message to test!", 26, &strview, 4, 6);
        cstr = fmstrv_to_cstr(&strview);
        assert(strcmp(" is a ", cstr) == 0 && "String View is not correct");
        FMFREE(cstr);
    }

    
    return 0;
}
