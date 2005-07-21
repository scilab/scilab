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

#ifdef lint5
	#include <sys/stdtypes.h>
	#define MALLOC(x) malloc(((size_t) x))
	#define FREE(x) if (x  != NULL) free((void *) x);
	#define REALLOC(x,y) realloc((void *) x,(size_t) y)
	#define CALLOC(x,y) calloc(x,(size_t) y)
#else
	#define MALLOC(x) malloc(((unsigned) x))
	#define FREE(x) if (x  != NULL) free((char *) x);
	#define REALLOC(x,y) realloc((char *) x,(unsigned) y)
	#define CALLOC(x,y) calloc( x,(unsigned) y)
#endif

#endif /* __SCI_MEM_ALLOC__ */
