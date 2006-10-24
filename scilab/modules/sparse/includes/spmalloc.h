#ifndef __SPMALLOC_H__
#define __SPMALLOC_H__

#include <stdlib.h>

#define SPMALLOC(x) malloc(((size_t) x))
#define SPALLOC(type,number)  ((type *)SPMALLOC((unsigned)(sizeof(type)*(number))))
#define SPREALLOC(ptr,type,number)  \
           ptr = (type *)realloc((char *)ptr,(unsigned)(sizeof(type)*(number)))

#define SPFREE(ptr) { if ((ptr) != NULL) {free((void *)(ptr)); (ptr) = NULL;}}


/* Calloc that properly handles allocating a cleared vector. */
#define SPCALLOC(ptr,type,number)                         \
{   int i; ptr = SPALLOC(type, number);                   \
    if (ptr != (type *)NULL)                            \
        for(i=(number)-1;i>=0; i--) ptr[i] = (type) 0;  \
}

#endif /*__SPMALLOC_H__*/
