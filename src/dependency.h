/* dependency - Dependecy tree definitions */
#ifdef IMPLEMENT_FMALL
#define IMPLEMENT_FMSTRV
#define IMPLEMENT_FMDSTR
#define IMPLEMENT_FMHASHMAP
#define INCLUDE_FMDARRAY
#define INCLUDE_FMCOMMON
#endif

#ifdef IMPLEMENT_FMSTRV
#define IMPLEMENT_FMDSTR
#define INCLUDE_FMSTRV
#endif

#ifdef IMPLEMENT_FMDSTR
#define INCLUDE_FMDSTR
#endif

#ifdef IMPLEMENT_FMHASHMAP
#define INCLUDE_FMHASHMAP
#endif

#ifdef INCLUDE_FMSTRV
#define INCLUDE_FMDSTR
#endif

#ifdef INCLUDE_FMDSTR
#define INCLUDE_FMDARRAY
#endif

#ifdef INCLUDE_FMHASHMAP
#define INCLUDE_FMCOMMON
#endif

#ifdef INCLUDE_FMDARRAY
#define INCLUDE_FMCOMMON
#endif
/* ends of dependency - Dependecy tree definitions */
