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
#include <string.h>
#include <stdlib.h>
#include "LaunchScilabSignal.h"
#include "Thread_Wrapper.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
__declspec(dllexport) __threadSignal		LaunchScilab = __StaticInitThreadSignal;
__declspec(dllexport) __threadSignalLock	*pLaunchScilabLock = NULL;

#include "mmapWindows.h"
#else
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS MAP_ANON
#endif
__threadSignal		LaunchScilab = __StaticInitThreadSignal;
__threadSignalLock	*pLaunchScilabLock = NULL;
#endif
/*--------------------------------------------------------------------------*/
void ReleaseLaunchScilabSignal(void)
{
#ifdef _MSC_VER
    /* http://msdn.microsoft.com/en-us/magazine/cc164040.aspx */
    if ((pLaunchScilabLock) && (pLaunchScilabLock->LockCount == -1))
#else
    if(pLaunchScilabLock)
#endif
    {
        __UnLockSignal(pLaunchScilabLock);
        munmap(pLaunchScilabLock, sizeof(__threadSignalLock));
        pLaunchScilabLock = NULL;
#ifdef _MSC_VER
        /* On Windows , we need to force value */
        LaunchScilab = __StaticInitThreadSignal;
#endif
    }
}
/*--------------------------------------------------------------------------*/
void InitializeLaunchScilabSignal(void)
{
    if (pLaunchScilabLock == NULL)
    {
        pLaunchScilabLock = mmap(0, sizeof(__threadSignalLock), PROT_READ | PROT_WRITE,MAP_SHARED |  MAP_ANONYMOUS, -1, 0);
        __InitSignal(&LaunchScilab);
        __InitSignalLock(pLaunchScilabLock);
        atexit(ReleaseLaunchScilabSignal);
    }
}
/*--------------------------------------------------------------------------*/
