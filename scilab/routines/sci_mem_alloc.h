/* Allan CORNET INRIA 2005 */

#ifndef __SCI_MEM_ALLOC__
#define __SCI_MEM_ALLOC__

#ifdef WIN32
	#include <Windows.h>
#endif

#ifdef lint5
	#include <sys/stdtypes.h>
	#define MALLOC(x) malloc(((size_t) x))
	#define FREE(x) if (x  != NULL) free((void *) x);
	#define REALLOC(x,y) realloc((void *) x,(size_t) y)
	#define CALLOC(x,y) calloc(x,(size_t) y)
#else
#if WIN32
	//#define MALLOC(x) VirtualAlloc(NULL,((unsigned) x),MEM_COMMIT,PAGE_READWRITE)
	//#define FREE(x) if (x  != NULL) VirtualFree((char *) x, 0, MEM_RELEASE);
	//#define CALLOC(x,y) VirtualAlloc(NULL,(unsigned) (x*sizeof(y)),MEM_COMMIT,PAGE_READWRITE)
	//#define REALLOC(x,y) VirtualAlloc((char *)x,(unsigned) y,MEM_COMMIT,PAGE_READWRITE)

	#define MALLOC(x) malloc(((unsigned) x))
	#define FREE(x) if (x  != NULL) free((char *) x);
	#define REALLOC(x,y) realloc((char *) x,(unsigned) y)
	#define CALLOC(x,y) calloc( x,(unsigned) y)
#else
	#define MALLOC(x) malloc(((unsigned) x))
	#define FREE(x) if (x  != NULL) free((char *) x);
	#define REALLOC(x,y) realloc((char *) x,(unsigned) y)
	#define CALLOC(x,y) calloc( x,(unsigned) y)
#endif
	
#endif

#endif /* __SCI_MEM_ALLOC__ */