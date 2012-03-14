/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <Windows.h>
#include "../includes/win_mem_alloc.h"
/*-----------------------------------------------------------------------------------*/
/* an interesting article about HeapAlloc,malloc, and OctAlloc */
/* bench show that HeapAlloc is faster than malloc on Windows */
/* http://denisbider.blogspot.com/2007/10/heap-allocation-on-multi-core-systems.html */
/* With VS 2010, we no more need to use heap, standard malloc is enough fast and compatible */
/* since malloc of VC runtime uses heapAlloc in internal */
/*-----------------------------------------------------------------------------------*/
#define FREE_FLAGS 0
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void *MyHeapRealloc(void *lpAddress, size_t dwSize, char *file, int line)
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

        if (NewPointer == NULL)
        {
#ifdef _DEBUG
            char MsgError[1024];
            wsprintf(MsgError,"REALLOC (1) Error File %s Line %d ", file, line);
            MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
#endif
        }
    }
    return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void *MyHeapAlloc(size_t dwSize, char *file, int line)
{
    LPVOID NewPointer = NULL;

    if (dwSize>0)
    {
        _try
        {
            NewPointer = malloc(dwSize);
            NewPointer = memset (NewPointer, 0, dwSize);
        }
        _except (EXCEPTION_EXECUTE_HANDLER)
        {
        }

        if (NewPointer == NULL)
        {
#ifdef _DEBUG
            char MsgError[1024];
            wsprintf(MsgError,"MALLOC (1) Error File %s Line %d ",file, line);
            MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
#endif
        }
    }
    else
    {
#ifdef _DEBUG
        char MsgError[1024];
        wsprintf(MsgError,"MALLOC (2) Error File %s Line %d ", file, line);
        MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
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
IMPORT_EXPORT_MALLOC_DLL void MyHeapFree(void *lpAddress, char *file, int line)
{
    _try
    {
        free(lpAddress);
    }
    _except (EXCEPTION_EXECUTE_HANDLER)
    {
#ifdef _DEBUG
        char MsgError[1024];
        wsprintf(MsgError,"FREE Error File %s Line %d ", file, line);
        MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
#endif
    }
}
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_MALLOC_DLL void *MyVirtualAlloc(size_t dwSize, char *file, int line)
{
    LPVOID NewPointer = NULL;

    if (dwSize>0)
    {
        _try
        {
            NewPointer = malloc(dwSize);
            NewPointer = memset (NewPointer, 0, dwSize);
        }
        _except (EXCEPTION_EXECUTE_HANDLER)
        {
        }

        if (NewPointer == NULL)
        {
#ifdef _DEBUG
            char MsgError[1024];
            wsprintf(MsgError,"MALLOC ( 1) Error File %s Line %d ", file, line);
            MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
#endif
        }
    }
    else
    {
#ifdef _DEBUG
        char MsgError[1024];
        wsprintf(MsgError,"MALLOC (2) Error File %s Line %d ", file, line);
        MessageBox(NULL,MsgError,"Error",MB_ICONSTOP | MB_OK);
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
IMPORT_EXPORT_MALLOC_DLL void MyVirtualFree(void *lpAddress, char *file, int line)
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
