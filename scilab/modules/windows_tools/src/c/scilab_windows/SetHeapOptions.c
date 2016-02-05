/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include <windows.h>
#include "SetHeapOptions.h"
/* http://msdn.microsoft.com/en-us/library/bb430720.aspx */
/*--------------------------------------------------------------------------*/
typedef BOOL (WINAPI *HSI) (HANDLE, HEAP_INFORMATION_CLASS , PVOID, SIZE_T);
/*--------------------------------------------------------------------------*/
BOOL SetHeapOptions(void)
{
#ifdef _DEBUG
    HMODULE hLib = LoadLibrary(TEXT("kernel32.dll"));
    if (hLib == NULL)
    {
        return FALSE;
    }
    else
    {
        BOOL fRet = FALSE;
        HSI pHsi = (HSI)GetProcAddress(hLib, "HeapSetInformation");
        if (!pHsi)
        {
            FreeLibrary(hLib);
            return FALSE;
        }
#ifndef HeapEnableTerminationOnCorruption
#define HeapEnableTerminationOnCorruption (HEAP_INFORMATION_CLASS)1
#endif

        fRet = (pHsi)(NULL, HeapEnableTerminationOnCorruption, NULL, 0) ? TRUE : FALSE;

        if (hLib)
        {
            FreeLibrary(hLib);
        }

        return fRet;
    }
#else
    return FALSE;
#endif
}
/*--------------------------------------------------------------------------*/
