/* Allan CORNET INRIA 2005 */

#ifndef __WIN_MEM_ALLOC__
#define __WIN_MEM_ALLOC__

#ifdef Top
#undef Top
#endif

#include <Windows.h>

#ifdef MALLOC
#undef MALLOC
#endif
#define MALLOC(x) MyHeapAlloc(x,__FILE__,__LINE__)

#ifdef FREE
#undef FREE
#endif
#define FREE(x) if (x  != NULL) MyHeapFree((char *)x,__FILE__,__LINE__);

#ifdef CALLOC
#undef CALLOC
#endif
#define CALLOC(x,y) MyHeapAlloc((x*y),__FILE__,__LINE__)


#ifdef REALLOC
#undef REALLOC
#endif
#define REALLOC(x,y) MyHeapRealloc(x, y,__FILE__,__LINE__)

#ifdef SCISTACKMALLOC
#undef SCISTACKMALLOC
#endif
#define SCISTACKMALLOC(x) MyVirtualAlloc((unsigned)x,__FILE__,__LINE__)

#ifdef SCISTACKFREE
#undef SCISTACKFREE
#endif
#define SCISTACKFREE(x) if (x  != NULL) MyVirtualFree((char *) x,__FILE__,__LINE__);


LPVOID MyHeapRealloc(LPVOID lpAddress,SIZE_T dwSize,char *fichier,int ligne);
LPVOID MyHeapAlloc(SIZE_T dwSize,char *fichier,int ligne);
void MyHeapFree(LPVOID lpAddress,char *fichier,int ligne);

LPVOID MyVirtualAlloc(SIZE_T dwSize,char *fichier,int ligne);
void MyVirtualFree(LPVOID lpAddress,char *fichier,int ligne);


#define Top C2F(vstk).top

#endif /* __WIN_MEM_ALLOC__ */
