#define IMPLEMENT_FMDSTR /* Implements all functions related to fmdstr*/
#include "fmstd.h"
fmdstr_t create_hi(void){
    fmdstr_t out = {0};
    out = fmdstr_from_cstr("hi!");
    return out;
}