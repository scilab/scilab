/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <windows.h>
#ifdef USE_F2C
#include "f2c.h"
#endif
/*-----------------------------------------------------------------------------------*/
#pragma comment(lib, "../../bin/libxml2.lib")
#pragma comment(lib, "../../bin/libintl.lib")
/*-----------------------------------------------------------------------------------*/
/* force linking fortran libraries */
#pragma comment(lib, "../../bin/blasplus.lib")
#pragma comment(lib, "../../bin/lapack.lib")
#pragma comment(lib, "../../libs/core_f.lib")
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
#ifdef USE_F2C
    return TRUE;
#else
    return FALSE;
#endif
}
/*-----------------------------------------------------------------------------------*/
