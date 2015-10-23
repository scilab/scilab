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

#include "InitializeTclTk.h"
#include "TerminateTclTk.h"


#ifdef _MSC_VER

#include "tcl.h"
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif
#include <windows.h>

#pragma comment(lib,"../../bin/libintl.lib")
#if defined(TCL_MAJOR_VERSION) && defined(TCL_MAJOR_VERSION)
#if TCL_MAJOR_VERSION == 8
#if TCL_MINOR_VERSION == 4
#pragma comment(lib,"../../bin/tcl84.lib")
#pragma comment(lib,"../../bin/tk84.lib")
#else
#if TCL_MINOR_VERSION == 5
#pragma comment(lib,"../../bin/tcl85.lib")
#pragma comment(lib,"../../bin/tk85.lib")
#else
#pragma message ("TCL/TK 8.4 or more required.")
#endif
#endif
#else
#pragma message ("TCL/TK 8.4 or more required.")
#endif
#endif
/*--------------------------------------------------------------------------*/
int WINAPI DllMain(HINSTANCE hInstance, DWORD reason, PVOID pvReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            TerminateTclTk();
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
    TerminateTclTk();
}
#endif

/*--------------------------------------------------------------------------*/

