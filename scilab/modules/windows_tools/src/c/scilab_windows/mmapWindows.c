/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <windows.h> 
#include "mmapWindows.h"
/*--------------------------------------------------------------------------*/
static int g_sl = 0;
/*--------------------------------------------------------------------------*/
/* Wait for spin lock */
static int slwait (int *sl) 
{
    while (InterlockedCompareExchange ((void **) sl, (void *) 1, (void *) 0) != 0)
    {
        Sleep (0);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
/* Release spin lock */
static int slrelease (int *sl) 
{
    InterlockedExchange (sl, 0);
    return 0;
}
/*--------------------------------------------------------------------------*/
/* getpagesize for windows */
static long getpagesize (void) 
{
    static long g_pagesize = 0;
    if (!g_pagesize) 
    {
        SYSTEM_INFO system_info;
        GetSystemInfo (&system_info);
        g_pagesize = system_info.dwPageSize;
    }
    return g_pagesize;
}
/*--------------------------------------------------------------------------*/
static long getregionsize (void) 
{
    static long g_regionsize = 0;
    if (! g_regionsize) 
    {
        SYSTEM_INFO system_info;
        GetSystemInfo (&system_info);
        g_regionsize = system_info.dwAllocationGranularity;
    }
    return g_regionsize;
}
/*--------------------------------------------------------------------------*/
void *mmap (void *ptr, long size, long prot, long type, long handle, long arg)
{ 
    static long g_pagesize;
    static long g_regionsize;

    /* Wait for spin lock */
    slwait (&g_sl);

    /* First time initialization */
    if (! g_pagesize)
    {
        g_pagesize = getpagesize ();
    }

    if (! g_regionsize) 
    {
        g_regionsize = getregionsize ();
    }

    /* Allocate this */
    ptr = VirtualAlloc (ptr, size, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
    if (! ptr) 
    {
        ptr = MMAP_FAILURE;
    }
    /* Release spin lock */
    slrelease (&g_sl);
    return ptr;
}
/*--------------------------------------------------------------------------*/
long munmap (void *ptr, long size)
{
    static long g_pagesize = 0;
    static long g_regionsize = 0;
    int rc = MUNMAP_FAILURE;

    /* Wait for spin lock */
    slwait (&g_sl);

    /* First time initialization */
    if (! g_pagesize)
    {
        g_pagesize = getpagesize ();
    }

    if (! g_regionsize) 
    {
        g_regionsize = getregionsize ();
    }

    /* Free this */
    if (! VirtualFree (ptr, 0, MEM_RELEASE))
    {
        /* Release spin lock */
        slrelease (&g_sl);
        return rc;
    }

    rc = 0;
    /* Release spin lock */
    slrelease (&g_sl);
    return rc;
}
/*--------------------------------------------------------------------------*/
