/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Allan CORNET
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
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
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
