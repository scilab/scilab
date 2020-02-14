/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO  -
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
#pragma comment(lib,"../../../../bin/hdf5.lib")
#pragma comment(lib,"../../../../bin/hdf5_cpp.lib")
#pragma comment(lib,"../../../../bin/hdf5_hl.lib")
#pragma comment(lib,"../../../../bin/hdf5_hl_cpp.lib")
/*--------------------------------------------------------------------------*/
int WINAPI DllMain(HINSTANCE hinstDLL, DWORD flag, LPVOID reserved)
{
    switch (flag)
    {
        case DLL_PROCESS_ATTACH :
        {
            Initialize_HDF5();
            break;
        }
        case DLL_PROCESS_DETACH :
        {
            Finalize_HDF5();
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
void __attribute__ ((constructor)) load_hdf5(void);
void __attribute__ ((destructor)) unload_hdf5(void);

void load_hdf5(void)
{
    Initialize_HDF5();
}

void unload_hdf5(void)
{
    // this finalize lead to a double free in HDF5
    //Finalize_HDF5();
}
#endif
/*--------------------------------------------------------------------------*/
