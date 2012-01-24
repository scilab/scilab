/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
    #include <Windows.h>
#else
    #include <sys/resource.h>
    #include "machine.h"
    #include "getmemory.h"

    #ifdef HAVE_LIMITS_H
        #include <limits.h>
    #elif !defined(LONG_MAX)
        #define LONG_MAX 2147483647L 
    #endif

    #ifdef USE_DYNAMIC_STACK
        #ifndef MAXLONG32
            #define MAXLONG32 2147483647L 
        #endif
    #endif

#endif

#include "../includes/getmaxMALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
IMPORT_EXPORT_MALLOC_DLL unsigned long GetLargestFreeMemoryRegion(void)
{
#if _WIN64
    /* we need to limit values to 32 bits for Scilab :( */

    /* Bug 10439 JVM reserves some space in 32 bit space */
    /* "Empiric" value for a Java Heap space to 256mb */
    /* It is not really a good workaround :/ */
    #define SECURITY_FREE_MEMORY 355483647 

    return MAXLONG32 - SECURITY_FREE_MEMORY;
#else
    #define SECURITY_FREE_MEMORY 1040000
    SYSTEM_INFO systemInfo;
    VOID *p = 0;
    MEMORY_BASIC_INFORMATION mbi;
    unsigned long largestSize = 0;

    GetSystemInfo(&systemInfo);

    while(p < systemInfo.lpMaximumApplicationAddress)
    {
        SIZE_T dwRet = VirtualQuery(p, &mbi, sizeof(mbi));
        if (dwRet > 0)
        {
            if (mbi.State == MEM_FREE)
            {
                if (largestSize < mbi.RegionSize)
                {
                    largestSize = (unsigned long) mbi.RegionSize;
                }
            }
            p = (void*) (((char*)p) + mbi.RegionSize);
        }
        else
        {
            p = (void*) (((char*)p) + systemInfo.dwPageSize);
        }
    }
    /* We remove a security size to be sure that MALLOC doesn't fails */
    if (largestSize > SECURITY_FREE_MEMORY) largestSize = largestSize - SECURITY_FREE_MEMORY;

    return largestSize;
#endif
}
/*-----------------------------------------------------------------------------------*/
#else
IMPORT_EXPORT_MALLOC_DLL unsigned long GetLargestFreeMemoryRegion(void)
{
#ifdef USE_DYNAMIC_STACK
    /* we need to limit values to 32 bits for Scilab :( */
    return MAXLONG32;
#else
    struct rlimit rlim;
    unsigned long largestSize, freeMem;

    /* HP-UX Use RLIMIT_AIO_MEM instead of RLIMIT_MEMLOCK */
    /* FIXME -- this should be an autoconf test to see which RLIMIT_foo is defined */
    #ifdef solaris
    getrlimit(RLIMIT_VMEM,&rlim);
    #elif defined(__NetBSD__) || defined(__DragonFly__)
    getrlimit(RLIMIT_RSS,&rlim);
    #else	
    getrlimit(RLIMIT_AS, &rlim);
    #endif
    if(rlim.rlim_max == RLIM_INFINITY)
    {
        largestSize = LONG_MAX;
    }
    else
    {
        largestSize = rlim.rlim_max;
    }

    freeMem = getfreememory()*1024;
    if(freeMem < largestSize)
    {
        return freeMem;
    }
    else
    {
        return largestSize;
    }

    return largestSize;
#endif /* #ifdef USE_DYNAMIC_STACK */
}
#endif
/*-----------------------------------------------------------------------------------*/
