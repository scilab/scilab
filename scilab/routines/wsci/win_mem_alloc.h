/* Allan CORNET INRIA 2005 */

#ifndef __WIN_MEM_ALLOC__
#define __WIN_MEM_ALLOC__

#include <Windows.h>

#ifdef MALLOC
#undef MALLOC
#endif
#define MALLOC(x) VirtualAlloc(NULL,((unsigned) x),MEM_COMMIT,PAGE_READWRITE)

#ifdef FREE
#undef FREE
#endif
#define FREE(x) if (x  != NULL) VirtualFree((char *) x, 0, MEM_RELEASE);

#ifdef CALLOC
#undef CALLOC
#endif
#define CALLOC(x,y) VirtualAlloc(NULL,(unsigned) (x*sizeof(y)),MEM_COMMIT,PAGE_READWRITE)

#ifdef REALLOC
#undef REALLOC
#endif
#define REALLOC(x,y) VirtualAlloc((char *)x,(unsigned) y,MEM_COMMIT,PAGE_READWRITE)
#endif /* __WIN_MEM_ALLOC__ */
