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

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <Windows.h>
#include "sci_malloc.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
/* an interesting article about HeapAlloc,malloc, and OctAlloc */
/* bench show that HeapAlloc is faster than malloc on Windows */
/* http://denisbider.blogspot.com/2007/10/heap-allocation-on-multi-core-systems.html */
/* With VS 2010, we no more need to use heap, standard malloc is enough fast and compatible */
/* since malloc of VC runtime uses heapAlloc in internal */
/*-----------------------------------------------------------------------------------*/
#define FREE_FLAGS 0
/*-----------------------------------------------------------------------------------*/
void *MyHeapRealloc(void *lpAddress, size_t dwSize, char *file, int line)
{
    LPVOID NewPointer = NULL;
    SIZE_T precSize = 0;

    if (lpAddress)
    {
        _try
        {
            NewPointer = realloc(lpAddress, dwSize);
        }
        _except (EXCEPTION_EXECUTE_HANDLER)
        {
        }
    }
    else
    {
        NewPointer = malloc(dwSize);
        NewPointer = memset (NewPointer, 0, dwSize);

#ifdef _DEBUG
        if (NewPointer == NULL)
        {
            sciprint("REALLOC (1) Error File %s Line %d ", file, line);
        }
#endif
    }
    return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
void *MyHeapAlloc(size_t dwSize, char *file, int line)
{
    LPVOID NewPointer = NULL;

    if (dwSize > 0)
    {
        _try
        {
            NewPointer = malloc(dwSize);
            NewPointer = memset (NewPointer, 0, dwSize);
        }
        _except (EXCEPTION_EXECUTE_HANDLER)
        {
        }

#ifdef _DEBUG
        if (NewPointer == NULL)
        {
            sciprint("MALLOC (1) Error File %s Line %d ", file, line);
        }
#endif
    }
    else
    {
#ifdef _DEBUG
        sciprint("MALLOC (2) Error File %s Line %d ", file, line);
#endif
        _try
        {
            NewPointer = malloc(dwSize);
            NewPointer = memset (NewPointer, 0, dwSize);
        }
        _except (EXCEPTION_EXECUTE_HANDLER)
        {
        }
    }
    return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
void MyHeapFree(void *lpAddress, char *file, int line)
{
    _try
    {
        free(lpAddress);
    }
    _except (EXCEPTION_EXECUTE_HANDLER)
    {
#ifdef _DEBUG
        sciprint("FREE Error File %s Line %d ", file, line);
#endif
    }
}
/*-----------------------------------------------------------------------------------*/
void *MyVirtualAlloc(size_t dwSize, char *file, int line)
{
    LPVOID NewPointer = NULL;

    if (dwSize > 0)
    {
        _try
        {
            NewPointer = malloc(dwSize);
            NewPointer = memset (NewPointer, 0, dwSize);
        }
        _except (EXCEPTION_EXECUTE_HANDLER)
        {
        }

#ifdef _DEBUG
        if (NewPointer == NULL)
        {
            sciprint("MALLOC ( 1) Error File %s Line %d ", file, line);
        }
#endif
    }
    else
    {
#ifdef _DEBUG
        sciprint("MALLOC (2) Error File %s Line %d ", file, line);
#endif

        _try
        {
            NewPointer = malloc(dwSize);
            NewPointer = memset (NewPointer, 0, dwSize);
        }
        _except (EXCEPTION_EXECUTE_HANDLER)
        {
        }

    }

    return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
void MyVirtualFree(void *lpAddress, char *file, int line)
{
    if (lpAddress)
    {
        _try
        {
            free(lpAddress);
        }
        _except (EXCEPTION_EXECUTE_HANDLER)
        {
        }
    }
}
/*-----------------------------------------------------------------------------------*/
void *MyHeapRealloc2(void *lpAddress, size_t dwSize, char *file, int line)
{
    return MyHeapRealloc(lpAddress, dwSize, file, line);
}

void *MyHeapAlloc2(size_t dwSize, char *file, int line)
{
    return MyHeapAlloc(dwSize, file, line);
}

void MyHeapFree2(void *lpAddress, char *file, int line)
{
    MyHeapFree(lpAddress, file, line);
}

void *MyVirtualAlloc2(size_t dwSize, char *file, int line)
{
    return MyVirtualAlloc(dwSize, file, line);
}

void MyVirtualFree2(void *lpAddress, char *file, int line)
{
    MyVirtualFree(lpAddress, file, line);
}

