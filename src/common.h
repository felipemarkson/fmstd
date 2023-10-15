/* common - Common definitions used in all library */
#ifdef INCLUDE_FMCOMMON
#ifndef FMCOMMON_H

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

#ifndef FMSIZE
#include <stddef.h>
#define FMSIZE size_t
#endif

#ifndef FMBOOL
#define FMBOOL unsigned int
#endif

#ifndef FMTRUE
#define FMTRUE ((FMBOOL)1)
#endif

#ifndef FMFALSE
#define FMFALSE ((FMBOOL)0)
#endif

#endif
#endif
/* ends of common - Common definitions used in all library */
