/* common - Common definitions used in all library */
#ifdef INCLUDE_FM_COMMON
#ifndef FM_COMMON_H

#ifndef NULL
#include <stddef.h>
#ifndef NULL
#error "Need to define NULL"
#endif
#endif

#ifndef FM_REALLOC
#include <stdlib.h>
#define FM_REALLOC realloc
#endif

#ifndef FM_FREE
#include <stdlib.h>
#define FM_FREE free
#endif

#ifndef FM_SIZE
#include <stddef.h>
#define FM_SIZE size_t
#endif

#ifndef FM_BOOL
#define FM_BOOL unsigned int
#endif

#ifndef FM_TRUE
#define FM_TRUE ((FM_BOOL)1)
#endif

#ifndef FM_FALSE
#define FM_FALSE ((FM_BOOL)0)
#endif

#endif
#endif
/* ends of common - Common definitions used in all library */
