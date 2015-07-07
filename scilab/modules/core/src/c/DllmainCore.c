/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
//for Visual Leak Detector in debug compilation mode
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif
/*--------------------------------------------------------------------------*/
#include <windows.h>
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
#pragma comment(lib, "../../../../bin/libxml2.lib")
#pragma comment(lib, "../../../../bin/libintl.lib")
#pragma comment(lib, "../../../../bin/blasplus.lib")
#pragma comment(lib, "../../../../bin/lapack.lib")
/*-----------------------------------------------------------------------------------*/
/* force linking fortran libraries */
#pragma comment(lib, "../../../../bin/core_f.lib")
#pragma comment(lib, "../../../../bin/elementary_functions_f.lib")
#pragma comment(lib, "../../../../bin/linpack_f.lib")
#pragma comment(lib, "../../../../bin/output_stream_f.lib")
/*-----------------------------------------------------------------------------------*/
extern int C2F(withf2c)(int *bOK);
/*-----------------------------------------------------------------------------------*/
int WINAPI DllMain (HINSTANCE hInstance , DWORD reason, PVOID pvReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}
/*-----------------------------------------------------------------------------------*/
BOOL BuildWithVS8ExpressF2C(void)
{
    BOOL bOK = FALSE;
    C2F(withf2c)(&bOK);
    return bOK;
}
/*-----------------------------------------------------------------------------------*/
