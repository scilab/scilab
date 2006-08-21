/* Allan CORNET INRIA 2005 */

#ifndef __SCI_MEM_ALLOC__
#define __SCI_MEM_ALLOC__

#if _MSC_VER
	#include <stdlib.h>
#endif

#ifdef MALLOC
#undef MALLOC
#endif

#ifdef FREE
#undef FREE
#endif

#ifdef CALLOC
#undef CALLOC
#endif

#ifdef REALLOC
#undef REALLOC
#endif

#if _MSC_VER
	#ifdef EXPORT_MALLOC_DLL
		#define IMPORT_EXPORT_MALLOC_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_MALLOC_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_MALLOC_DLL extern 
#endif

IMPORT_EXPORT_MALLOC_DLL void * MyReAlloc(void * lpAddress,int dwSize,char *fichier,int ligne);
IMPORT_EXPORT_MALLOC_DLL void * MyAlloc(unsigned int dwSize,char *fichier,int ligne);
IMPORT_EXPORT_MALLOC_DLL void * MyCalloc(unsigned  int x, unsigned int y, char *fichier,int ligne);
IMPORT_EXPORT_MALLOC_DLL void  MyFree(void *x, char *fichier,int ligne);

#ifndef _MSC_VER
 
 #ifdef lint5
   #include <sys/stdtypes.h>
   #define MALLOC(x) MyAlloc((size_t)x,__FILE__,__LINE__)
   #define FREE(x) if (x  != NULL) MyFree((void *) x,__FILE__,__LINE__);
   #define REALLOC(x,y) MyReAlloc((void *)x, y,__FILE__,__LINE__)
   #define CALLOC(x,y) MyCalloc((size_t) x, (size_t) y,__FILE__,__LINE__)

   /* Only for Scilab Stacksize use VirtualAlloc on Window */
   #define SCISTACKMALLOC(x) MyAlloc((size_t)x,__FILE__,__LINE__)
   #define SCISTACKFREE(x) if (x  != NULL) MyFree((void *) x,__FILE__,__LINE__);

 #else
   #define MALLOC(x) MyAlloc((unsigned)x,__FILE__,__LINE__)
   #define FREE(x) if (x  != NULL) MyFree((char *) x,__FILE__,__LINE__);
   #define REALLOC(x,y) MyReAlloc((char *)x, y,__FILE__,__LINE__)
   #define CALLOC(x,y) MyCalloc((unsigned) x, (unsigned) y,__FILE__,__LINE__)

    /* Only for Scilab Stacksize use VirtualAlloc on Window */
   #define SCISTACKMALLOC(x) MyAlloc((unsigned)x,__FILE__,__LINE__)
   #define SCISTACKFREE(x) if (x  != NULL) MyFree((char *) x,__FILE__,__LINE__);


 #endif
#else
	#define MALLOC(x) MyAlloc((unsigned)x,__FILE__,__LINE__)
	#define FREE(x) if (x  != NULL) MyFree((char *) x,__FILE__,__LINE__);
	#define REALLOC(x,y) MyReAlloc((char *)x, y,__FILE__,__LINE__)
	#define CALLOC(x,y) MyCalloc((unsigned) x, (unsigned) y,__FILE__,__LINE__)

#endif	

#endif /* __SCI_MEM_ALLOC__ */
