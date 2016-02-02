/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __SPMALLOC_H__
#define __SPMALLOC_H__

#include <stdlib.h>
#include "sci_malloc.h"
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
