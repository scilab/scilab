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

#ifndef WIN32
 extern void * MyReAlloc(void * lpAddress,int dwSize,char *fichier,int ligne);
 extern void * MyAlloc(unsigned int dwSize,char *fichier,int ligne);
 extern void * MyCalloc(unsigned  int x, unsigned int y, char *fichier,int ligne);

 #ifdef lint5
   #include <sys/stdtypes.h>
   #define MALLOC(x) MyAlloc((size_t)x,__FILE__,__LINE__)
   #define FREE(x) if (x  != NULL) free((void *) x);
   #define REALLOC(x,y) MyReAlloc((void *)x, y,__FILE__,__LINE__)
   #define CALLOC(x,y) MyCalloc((size_t) x, (size_t) y,__FILE__,__LINE__)
 #else
   #define MALLOC(x) MyAlloc((unsigned)x,__FILE__,__LINE__)
   #define FREE(x) if (x  != NULL) free((char *) x);
   #define REALLOC(x,y) MyReAlloc((char *)x, y,__FILE__,__LINE__)
   #define CALLOC(x,y) MyCalloc((unsigned) x, (unsigned) y,__FILE__,__LINE__)
 #endif
#else
	#define MALLOC(x) malloc(((unsigned) x))
	#define FREE(x) if (x  != NULL) free((char *) x);
	#define REALLOC(x,y) realloc((char *) x,(unsigned) y)
	#define CALLOC(x,y) calloc( x,(unsigned) y)
#endif	

#endif /* __SCI_MEM_ALLOC__ */
