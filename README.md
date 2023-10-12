# Felipe Monteiro's C standard library

Single-file public domain library for C/C++. Compatible with C89 (ANSI C).

- Generic dynamic array
- Dynamic String
- Can be compiled without any other libc dependencies. 

## How to use

This library is a "header-only" using the same approach of the
[Nothing's stb libraries](https://github.com/nothings/stb/), which means that you need
only the `fm_std.h` file.
However, instead of having different header-files, this library put all together and you
choose what you want to include/implement in your C/C++ file using the `INCLUDE_FM_*`or
`IMPLEMENT_FM_*`  macro.

### Example

Suposse you have in a folder the files `fm_std.h`, `hi.c`, and `main.c`.

```c
/* hi.c */
#define IMPLEMENT_FM_DSTR /* Implements all functions related to fm_dstr */
#include "fm_std.h"
fm_dstr_t create_hi(void){
    fm_dstr_t out = {0};
    out = fm_dstr_from_cstr("hi!");
    return out;
}
```

```c
/* main.c */
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
```

The compilation can be done by:
```console
$ clang -c hi.c
$ clang -c main.c
$ clang hi.o main.o -o main
```
## Compiling without any other libc dependency

The `fm_std.h` is divided in some blocks, the block `common` defines the basic
functions and macros that should be provided by the user. For example, the macro
`FM_REALLOC` by default will use the `realloc` from `stdlib.h`, but if you want to
define your own `realloc` function, you can define the macro `FM_REALLOC` to whatever
fuction you want with the same signature of `realloc` before the `fm_std.h` inclusion.

```c
#define FM_REALLOC my_realloc
#define INCLUDE_FM_DARRAY
#include "fm_std.h"
void* my_realloc(void* ptr, size_t size){ return NULL; }
/* your code */
```

You can see an example in `test/test_realloc.c`.

All the macros that you can replace can be found on `src/common.h`.

## License

The source code are licesed under MIT No Attribution, a public domain equivalent
license.

