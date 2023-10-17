#include <stdio.h> /* printf */

#define INCLUDE_FMDSTR /* Only includes, not implement */
#include "fmstd.h"
fmdstr_t create_hi(void); /* create_hi is implemented in hi.c */

int main(void){
    fmdstr_t hi = {0};
    char* cstr = {0};

    hi = create_hi(); /* create_hi is implemented in hi.c */
    cstr = fmdstr_to_cstr(&hi); /* fmdstr_to_cstr is implemented in hi.c */
    printf("%s\n", cstr);
    FMFREE(hi.elems); /* FMFREE is a macro defined in fmstd.h */
    FMFREE(cstr);
    return 0;
}