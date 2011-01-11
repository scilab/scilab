/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "LocaleNameToLCID_Windows.h"
/*--------------------------------------------------------------------------*/
/* ONLY for Windows */
/* Some old XP without SP2 or more do not have this function ... */
/*--------------------------------------------------------------------------*/
static HINSTANCE KernelDll = NULL;
/*--------------------------------------------------------------------------*/
typedef LCID (WINAPI * LocaleNameToLCIDPROC)(LPCWSTR lpName, DWORD dwFlags);
/*--------------------------------------------------------------------------*/
LCID dllLocaleNameToLCID(LPCWSTR lpName, DWORD dwFlags)
{
    if (KernelDll == NULL) KernelDll = LoadLibrary ("kernel32.dll"); 
    if (KernelDll)
    {
        LocaleNameToLCIDPROC ptrFunctionLocaleNameToLCID = (LocaleNameToLCIDPROC)GetProcAddress(KernelDll, "LocaleNameToLCID");
        if (ptrFunctionLocaleNameToLCID)
        {
            return (LCID)(ptrFunctionLocaleNameToLCID)(lpName, dwFlags);
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
