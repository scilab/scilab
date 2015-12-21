/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "FieldsManager.hxx"
#ifdef _MSC_VER
extern "C"
{
#include <windows.h>
    int WINAPI DllMain(HINSTANCE hInstance, DWORD reason, PVOID pvReserved);
}
/*--------------------------------------------------------------------------*/
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../../../bin/libintl.lib")
/*--------------------------------------------------------------------------*/
int WINAPI DllMain(HINSTANCE hInstance, DWORD reason, PVOID pvReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            org_modules_completion::FieldsManager::clearFieldsGetter();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}
#else
__attribute__ ((constructor)) static void load(void);
__attribute__ ((destructor)) static void unload(void);

void load(void)
{
}

void unload(void)
{
    org_modules_completion::FieldsManager::clearFieldsGetter();
}
#endif

/*--------------------------------------------------------------------------*/

