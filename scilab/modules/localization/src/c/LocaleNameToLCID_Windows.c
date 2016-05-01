/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
    if (KernelDll == NULL)
    {
        KernelDll = LoadLibrary ("kernel32.dll");
    }
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
