
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
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
#include "machine.h" /* C2F */
/*--------------------------------------------------------------------------*/
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../../../bin/libintl.lib")
/*--------------------------------------------------------------------------*/
/* defines & exports fortran COMMON in C */
/* required by F2C */

__declspec (dllexport) struct
{
    int iero;
} C2F(iercorr);

__declspec (dllexport) struct
{
    double pi2, dev;
    int nfcns, ngrid;
} C2F(rem001);

__declspec (dllexport) struct
{
    int niter, iout;
} C2F(oops);

__declspec (dllexport) struct
{
    char namex[25], namey[25];
} C2F(corrname);


__declspec (dllexport) struct
{
    int kgxtop, kgytop, ksec, kisc;
} C2F(corradr);

__declspec (dllexport) struct
{
    int itxcorr, itycorr;
} C2F(corrtyp);

__declspec (dllexport) struct
{
    int k;
} C2F(tg02bd);
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

