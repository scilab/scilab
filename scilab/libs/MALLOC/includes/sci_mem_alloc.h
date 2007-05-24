/* Allan CORNET INRIA 2005 */

#ifndef __SCI_MEM_ALLOC__
#define __SCI_MEM_ALLOC__

#undef Top

#ifdef _MSC_VER
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

#ifdef NULL
	#undef NULL
#endif
#define NULL 0

#ifdef _MSC_VER
#ifdef EXPORT_MALLOC_DLL
#define IMPORT_EXPORT_MALLOC_DLL __declspec(dllexport)
#else
#define IMPORT_EXPORT_MALLOC_DLL __declspec(dllimport)
#endif
#else
#define IMPORT_EXPORT_MALLOC_DLL extern
#endif


#ifndef _MSC_VER

/**
 * Reallocate the memory
 * Unix/Linux only
 *
 * @param lpAddress Address 
 * @param dwSize Size
 * @param file Where the function is called (used debug) 
 * @param line  
 * @return <ReturnValue>
 */
 void * MyReAlloc(void * lpAddress,int dwSize,char *file,int line);
 void * MyAlloc(unsigned int dwSize,char *file,int line);
 void * MyCalloc(unsigned  int x, unsigned int y, char *file,int line);
 void MyFree(void *lpAddress,char *file,int line);

 #ifdef lint5
   #include <sys/stdtypes.h>
   #define MALLOC(x) MyAlloc((size_t)x,__FILE__,__LINE__)
   #define FREE(x) if (x  != NULL) free((void *) x);
   #define REALLOC(x,y) MyReAlloc((void *)x, y,__FILE__,__LINE__)
   #define CALLOC(x,y) MyCalloc((size_t) x, (size_t) y,__FILE__,__LINE__)

   /* Only for Scilab Stacksize use VirtualAlloc on Window */
   #define SCISTACKMALLOC(x) MyAlloc((size_t)x,__FILE__,__LINE__)
   #define SCISTACKFREE(x,y) if (x  != NULL) MyFree((void *) x,__FILE__,__LINE__);
 #else
   #define MALLOC(x) MyAlloc((unsigned)x,__FILE__,__LINE__)
   #define FREE(x) if (x  != NULL) free((char *) x);
   #define REALLOC(x,y) MyReAlloc((char *)x, y,__FILE__,__LINE__)
   #define CALLOC(x,y) MyCalloc((unsigned) x, (unsigned) y,__FILE__,__LINE__)

    /* Only for Scilab Stacksize use VirtualAlloc on Window */
   #define SCISTACKMALLOC(x) MyAlloc((unsigned)x,__FILE__,__LINE__)
   #define SCISTACKFREE(x) if (x  != NULL) MyFree((char *) x,__FILE__,__LINE__);
 #endif
#else
	#define MALLOC(x) malloc(((unsigned) x))
	#define FREE(x) if (x  != NULL) free((char *) x);
	#define REALLOC(x,y) realloc((char *) x,(unsigned) y)
	#define CALLOC(x,y) calloc( x,(unsigned) y)
	#ifndef _MSC_VER
	#define SCISTACKMALLOC(x) malloc(((unsigned) x))
	#define SCISTACKFREE(x) if (x  != NULL) free((char *) x);
	#endif


#endif	

#define Top C2F(vstk).top

#endif /* __SCI_MEM_ALLOC__ */
