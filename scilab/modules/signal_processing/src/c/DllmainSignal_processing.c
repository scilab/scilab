
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <windows.h> 
#include "machine.h" /* C2F */
/*--------------------------------------------------------------------------*/ 
#pragma comment(lib,"../../../../bin/libintl.lib")
/*--------------------------------------------------------------------------*/ 
/* defines & exports fortran COMMON in C */
/* required by F2C */

__declspec (dllexport) struct {
    int iero;
} C2F(iercorr);

__declspec (dllexport) struct {
    double pi2, dev;
    int nfcns, ngrid;
} C2F(rem001);

__declspec (dllexport) struct {
    int niter, iout;
} C2F(oops);

__declspec (dllexport) struct {
    char namex[25], namey[25];
} C2F(corrname);


__declspec (dllexport) struct {
    int kgxtop, kgytop, ksec, kisc;
} C2F(corradr);

__declspec (dllexport) struct {
    int itxcorr, itycorr;
} C2F(corrtyp);

__declspec (dllexport) struct {
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

