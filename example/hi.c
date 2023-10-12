#define IMPLEMENT_FM_DSTR /* Implements all functions related to fm_dstr*/
#include "fm_std.h"
fm_dstr_t create_hi(void){
    fm_dstr_t out = {0};
    out = fm_dstr_from_cstr("hi!");
    return out;
}