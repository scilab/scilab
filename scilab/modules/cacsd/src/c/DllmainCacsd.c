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

#include <windows.h>
#include "machine.h"
/*--------------------------------------------------------------------------*/
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../../../bin/libintl.lib")
#pragma comment(lib,"../../../../bin/blasplus.lib")
#pragma comment(lib,"../../../../bin/lapack.lib")
/*--------------------------------------------------------------------------*/
/* We force fortran COMMON definitions */

__declspec(dllexport) struct
{
    int io, info, ll;
} C2F(sortie);

__declspec(dllexport) struct
{
    int nall1;
} C2F(comall);

__declspec(dllexport) struct
{
    double t;
} C2F(temps);

__declspec(dllexport) struct
{
    double gnrm;
} C2F(no2f);

__declspec(dllexport) struct
{
    int info, i1;
} C2F(arl2c);
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/

