/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO  -
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_hdf5.h"

#ifdef _MSC_VER
#include <windows.h>
/*--------------------------------------------------------------------------*/
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../../../bin/libintl.lib")
#pragma comment(lib,"../../../../bin/hdf5dll.lib")
#pragma comment(lib,"../../../../bin/hdf5_hldll.lib")
/*--------------------------------------------------------------------------*/
int WINAPI DllMain(HINSTANCE hinstDLL, DWORD flag, LPVOID reserved)
{
    switch (flag)
    {
        case DLL_PROCESS_ATTACH :
        {
            Initialize();
            break;
        }
        case DLL_PROCESS_DETACH :
        {
            Finalize();
            break;
        }
        case DLL_THREAD_ATTACH :
        {
            break;
        }
        case DLL_THREAD_DETACH :
        {
            break;
        }
        default :
        {
            return 0;
        }
    }
    return 1;
}
#else
void __attribute__ ((constructor)) load(void);
void __attribute__ ((destructor)) unload(void);

void load(void)
{
    Initialize();
}

void unload(void)
{
    Finalize();
}
#endif
/*--------------------------------------------------------------------------*/

