#include <stdio.h> /* printf */

#define INCLUDE_FM_DSTR /* Only includes, not implement */
#include "fm_std.h"
fm_dstr_t create_hi(void); /* create_hi is implemented in hi.c */

int main(void){
    fm_dstr_t hi = {0};
    char* cstr = {0};

    hi = create_hi(); /* create_hi is implemented in hi.c */
    cstr = fm_dstr_to_cstr(&hi); /* fm_dstr_to_cstr is implemented in hi.c */
    printf("%s\n", cstr);
    FM_FREE(hi.elems); /* FM_FREE is a macro defined in fm_std.h */
    return 0;
}