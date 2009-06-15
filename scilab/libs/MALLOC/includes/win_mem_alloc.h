/* Allan CORNET INRIA 2005 */

#ifndef __WIN_MEM_ALLOC__
#define __WIN_MEM_ALLOC__

#undef Top

#include <Windows.h>

#ifdef NULL
	#undef NULL
#endif
#define NULL 0

#include "ExportImport.h" /* IMPORT_EXPORT_MALLOC_DLL */

IMPORT_EXPORT_MALLOC_DLL LPVOID MyHeapRealloc(LPVOID lpAddress,SIZE_T dwSize,char *fichier,int ligne);
IMPORT_EXPORT_MALLOC_DLL LPVOID MyHeapAlloc(SIZE_T dwSize,char *fichier,int ligne);
IMPORT_EXPORT_MALLOC_DLL void MyHeapFree(LPVOID lpAddress,char *fichier,int ligne);

IMPORT_EXPORT_MALLOC_DLL LPVOID MyVirtualAlloc(SIZE_T dwSize,char *fichier,int ligne);
IMPORT_EXPORT_MALLOC_DLL void MyVirtualFree(LPVOID lpAddress,char *fichier,int ligne);


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
#define CALLOC(x,y) MyHeapAlloc(((x)*(y)),__FILE__,__LINE__)


#ifdef REALLOC
#undef REALLOC
#endif
#define REALLOC(x,y) MyHeapRealloc(x, y,__FILE__,__LINE__)

#ifdef SCISTACKMALLOC
#undef SCISTACKMALLOC
#endif
#define SCISTACKMALLOC(x) MyVirtualAlloc((unsigned long)x,__FILE__,__LINE__)

#ifdef SCISTACKFREE
#undef SCISTACKFREE
#endif
#define SCISTACKFREE(x) if (x  != NULL) MyVirtualFree((char *) x,__FILE__,__LINE__);

#define Top C2F(vstk).top

#endif /* __WIN_MEM_ALLOC__ */
