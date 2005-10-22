/* Allan CORNET INRIA 2005 */

#ifndef __SCI_MEM_ALLOC__
#define __SCI_MEM_ALLOC__

#if WIN32
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

 extern void * MyReAlloc(void * lpAddress,int dwSize,char *fichier,int ligne);
 extern void * MyAlloc(unsigned int dwSize,char *fichier,int ligne);
 extern void * MyCalloc(unsigned  int x, unsigned int y, char *fichier,int ligne);
 extern void  MyFree(void *x, char *fichier,int ligne);

#ifndef WIN32
 
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
