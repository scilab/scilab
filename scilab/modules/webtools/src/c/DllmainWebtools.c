/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO  -
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2017 - ESI-Group
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "webtools.h"

#ifdef _MSC_VER
#include <windows.h>

#pragma comment(lib, "../../bin/libcurl.lib")
#pragma comment(lib, "../../bin/libintl.lib")
/*--------------------------------------------------------------------------*/
//for Visual Leak Detector in debug compilation mode
//#define DEBUG_VLD
#if defined(DEBUG_VLD) && defined(_DEBUG)
#include <vld.h>
#endif
/*--------------------------------------------------------------------------*/
int WINAPI DllMain(HINSTANCE hinstDLL, DWORD flag, LPVOID reserved)
{
    switch (flag)
    {
        case DLL_PROCESS_ATTACH :
        {
            Initialize_Webtools();
            break;
        }
        case DLL_PROCESS_DETACH :
        {
            Finalize_Webtools();
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
void __attribute__ ((constructor)) load_curl(void);
void __attribute__ ((destructor)) unload_curl(void);

void load_curl(void)
{
    Initialize_Webtools();
}

void unload_curl(void)
{
    Finalize_Webtools();
}
#endif
/*--------------------------------------------------------------------------*/
