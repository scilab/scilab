/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __WIN_MEM_ALLOC__
#define __WIN_MEM_ALLOC__

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#include <crtdefs.h>


void *MyHeapRealloc(void *lpAddress, size_t dwSize, char *file, int line);
void *MyHeapAlloc(size_t dwSize, char *file, int line);
void MyHeapFree(void *lpAddress, char *file, int line);
void *MyVirtualAlloc(size_t dwSize, char *file, int line);
void MyVirtualFree(void *lpAddress, char *file, int line);

void *MyHeapRealloc2(void *lpAddress, size_t dwSize, char *file, int line);
void *MyHeapAlloc2(size_t dwSize, char *file, int line);
void MyHeapFree2(void *lpAddress, char *file, int line);
void *MyVirtualAlloc2(size_t dwSize, char *file, int line);
void MyVirtualFree2(void *lpAddress, char *file, int line);


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

#endif /* __WIN_MEM_ALLOC__ */
