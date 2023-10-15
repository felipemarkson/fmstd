# Felipe Monteiro's C standard library

Single-file public domain library for C/C++. Compatible with C89 (ANSI C).

- Generic dynamic array: `fmdarray`
- Dynamic String: `fmdstr`
- String View: `fmstrv`
- Can be compiled without any other libc dependencies. 

## How to use

This library is a "header-only" using the same approach of the
[STB libraries](https://github.com/nothings/stb/), which means that you need
only the `fmstd.h` file.
However, instead of having different header-files, this library put all together and you
choose what you want to include/implement in your C/C++ file using the `INCLUDE_FM*`or
`IMPLEMENT_FM*`  macro.

### Example

Suposse you have in a folder the files `fmstd.h`, `hi.c`, and `main.c`.

```c
/* hi.c */
#define IMPLEMENT_FMDSTR /* Implements all functions related to fmdstr */
#include "fmstd.h"
fmdstr_t create_hi(void){
    fmdstr_t out = {0};
    out = fmdstr_from_cstr("hi!");
    return out;
}
```

```c
/* main.c */
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

The `fmstd.h` is divided in some blocks, the block `common` defines the basic
functions and macros that should be provided by the user. For example, the macro
`FMREALLOC` by default will use the `realloc` from `stdlib.h`, but if you want to
define your own `realloc` function, you can define the macro `FMREALLOC` to whatever
fuction you want with the same signature of `realloc` before the `fmstd.h` inclusion.

```c
#define FMREALLOC my_realloc
#define INCLUDE_FMDARRAY
#include "fmstd.h"
void* my_realloc(void* ptr, size_t size){ return NULL; }
/* your code */
```

You can see an example in `test/test_realloc.c`.

All the macros that you can replace can be found on `src/common.h`.

## License

The source code are licesed under MIT No Attribution, a public domain equivalent
license.
