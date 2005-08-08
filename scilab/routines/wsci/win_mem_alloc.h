/* Allan CORNET INRIA 2005 */

#ifndef __WIN_MEM_ALLOC__
#define __WIN_MEM_ALLOC__

#include <Windows.h>

#ifdef MALLOC
#undef MALLOC
#endif
#define MALLOC(x) MyVirtualAlloc(x,__FILE__,__LINE__)

#ifdef FREE
#undef FREE
#endif
#define FREE(x) if (x  != NULL) VirtualFree((char *) x, 0, MEM_RELEASE);

#ifdef CALLOC
#undef CALLOC
#endif
#define CALLOC(x,y) MyVirtualAlloc((unsigned) (x*y),__FILE__,__LINE__)


#ifdef REALLOC
#undef REALLOC
#endif
#define REALLOC(x,y) VirtualReAlloc((char *)x, y,__FILE__,__LINE__)

LPVOID VirtualReAlloc(LPVOID lpAddress,SIZE_T dwSize,char *fichier,int ligne);
LPVOID MyVirtualAlloc(SIZE_T dwSize,char *fichier,int ligne);
#endif /* __WIN_MEM_ALLOC__ */
