/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include <Psapi.h>
#include "getScilabPreference.h"
#else
#include <sys/resource.h>
#include "machine.h"
#include "getmemory.h"

#ifdef HAVE_LIMITS_H
#include <limits.h>
#elif !defined(LONG_MAX)
#define LONG_MAX 2147483647L
#endif

#ifndef _MSC_VER
#ifndef MAXLONG32
#define MAXLONG32 2147483647L
#endif
#endif

#endif

#include "getmaxMALLOC.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
unsigned long GetLargestFreeMemoryRegion(void)
{

#if _WIN64

    const ScilabPreferences* prefs =  getScilabPreferences();
    /* we need to limit values to 32 bits for Scilab :( */

    /* Bug 10439 JVM reserves some space in 32 bit space */
    /* "Empiric" value for a Java Heap space to 256mb */
    /* It is not really a good workaround :/ */
    unsigned long SECURITY_FREE_MEMORY = (atoi(prefs->heapSize) + 85) * (1024 * 1024);
#else
#define SECURITY_FREE_MEMORY 1040000
#endif

    unsigned long realSize = 0;

    //due to scilab stack limitation this value must not be > MAX_INT
    realSize = 0x7fffffff;

    if (realSize > SECURITY_FREE_MEMORY)
    {
        realSize -= SECURITY_FREE_MEMORY;
    }
    return realSize;
}
/*-----------------------------------------------------------------------------------*/
#else
unsigned long GetLargestFreeMemoryRegion(void)
{
#ifndef _MSC_VER
    /* we need to limit values to 32 bits for Scilab :( */
    return MAXLONG32;
#else
    struct rlimit rlim;
    unsigned long largestSize, freeMem;

    /* HP-UX Use RLIMIT_AIO_MEM instead of RLIMIT_MEMLOCK */
    /* FIXME -- this should be an autoconf test to see which RLIMIT_foo is defined */
#ifdef solaris
    getrlimit(RLIMIT_VMEM, &rlim);
#elif defined(__NetBSD__) || defined(__DragonFly__)
    getrlimit(RLIMIT_RSS, &rlim);
#else
    getrlimit(RLIMIT_AS, &rlim);
#endif
    if (rlim.rlim_max == RLIM_INFINITY)
    {
        largestSize = LONG_MAX;
    }
    else
    {
        largestSize = rlim.rlim_max;
    }

    freeMem = getfreememory() * 1024;
    if (freeMem < largestSize)
    {
        return freeMem;
    }
    else
    {
        return largestSize;
    }

    return largestSize;
#endif /* #ifdef _MSC_VER (was: USE_DYNAMIC_STACK) */
}
#endif
/*-----------------------------------------------------------------------------------*/
