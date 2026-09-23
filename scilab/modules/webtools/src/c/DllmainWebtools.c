/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
#pragma comment(lib, "../../bin/libxml2.lib")
#pragma comment(lib, "../../bin/libintl.lib")
#if defined(_DEBUG)
#pragma comment(lib, "../../libs/cppserver/lib/asiod.lib")
#pragma comment(lib, "../../libs/cppserver/lib/cppcommond.lib")
#pragma comment(lib, "../../libs/cppserver/lib/cppserverd.lib")
#pragma comment(lib, "../../libs/cppserver/lib/fmtd.lib")
#else
#pragma comment(lib, "../../libs/cppserver/lib/asio.lib")
#pragma comment(lib, "../../libs/cppserver/lib/cppcommon.lib")
#pragma comment(lib, "../../libs/cppserver/lib/cppserver.lib")
#pragma comment(lib, "../../libs/cppserver/lib/fmt.lib")
#endif
#pragma comment(lib, "../../libs/openssl/lib/libcrypto.lib")
#pragma comment(lib, "../../libs/openssl/lib/libssl.lib")

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
