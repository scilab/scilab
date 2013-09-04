/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SPMALLOC_H__
#define __SPMALLOC_H__

#include <stdlib.h>
#include "MALLOC.h"
#define SPMALLOC(x) MALLOC(((size_t) x))
#define SPALLOC(type,number)  ((type *)SPMALLOC((unsigned)(sizeof(type)*(number))))
#define SPREALLOC(ptr,type,number)  \
           ptr = (type *)REALLOC((char *)ptr,(unsigned)(sizeof(type)*(number)))

#define SPFREE(ptr) { if ((ptr) != NULL) {FREE((void *)(ptr)); (ptr) = NULL;}}


/* Calloc that properly handles allocating a cleared vector. */
#define SPCALLOC(ptr,type,number)                         \
{   int i; ptr = SPALLOC(type, number);                   \
    if (ptr != (type *)NULL)                            \
        for(i=(number)-1;i>=0; i--) ptr[i] = (type) 0;  \
}

#endif /*__SPMALLOC_H__*/
