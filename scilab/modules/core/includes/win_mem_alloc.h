/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
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

#ifndef __WIN_MEM_ALLOC__
#define __WIN_MEM_ALLOC__

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#define NOMINMAX
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
