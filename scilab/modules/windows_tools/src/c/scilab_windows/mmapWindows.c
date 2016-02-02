/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <windows.h>
#include "mmapWindows.h"
/*--------------------------------------------------------------------------*/
#ifdef _WIN64
typedef LONG64 LOCKLONG;
#else
typedef LONG LOCKLONG;
#endif
static LOCKLONG g_sl = 0;
/*--------------------------------------------------------------------------*/
/* Wait for spin lock */
static int slwait (LOCKLONG *sl)
{

    LOCKLONG Destination = *sl;
    LONG Exchange = 1;
    LONG Comparand = 0;
#if _WIN64
    while (InterlockedCompareExchange64(&Destination, Exchange, Comparand))

#else
    while (InterlockedCompareExchange (&Destination, Exchange, Comparand))
#endif
    {
        Sleep (0);
    }
    *sl = Destination;
    return 0;
}
/*--------------------------------------------------------------------------*/
/* Release spin lock */
static int slrelease (LOCKLONG *sl)
{
    InterlockedExchange ((volatile LONG*)sl, 0);
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
