/* common - Common definitions used in all library */
#ifdef INCLUDE_FMCOMMON
#ifndef FMCOMMON_H
#define FMCOMMON_H

#ifndef NULL
#include <stddef.h>
#ifndef NULL
#error "Need to define NULL"
#endif
#endif

#ifndef FMREALLOC
#include <stdlib.h>
#define FMREALLOC realloc
#endif

#ifndef FMFREE
#include <stdlib.h>
#define FMFREE free
#endif

#ifndef FMSTRLEN
#include <string.h>
#define FMSTRLEN strlen
#endif

#ifndef FMMEMCPY
#include <string.h>
#define FMMEMCPY memcpy
#endif

#ifndef FMMEMSET
#include <string.h>
#define FMMEMSET memset
#endif

#ifndef FMSIZE
#include <stddef.h>
#define FMSIZE size_t
#include <stdint.h>
#define FMSIZE_MAX SIZE_MAX
#endif

#ifndef FMBOOL
#include <stdbool.h>
#define FMBOOL bool
#define FMTRUE true
#define FMFALSE false
#endif

#ifndef FMBYTE
#include <stdint.h>
#define FMBYTE uint8_t
#endif

#ifndef FMDATAPTR
#include <stdint.h>
#define FMDATAPTR uintptr_t
#endif

#ifndef FMASSERT
#include <assert.h>
#define FMASSERT assert
#endif

#endif
#endif
/* ends of common - Common definitions used in all library */
