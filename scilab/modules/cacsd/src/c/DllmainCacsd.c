/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <windows.h>
#include "machine.h"
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

