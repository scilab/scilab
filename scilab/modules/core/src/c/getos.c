/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#include "../../../libs/GetWindowsVersion/GetWindowsVersion.h"
#else
#include <sys/utsname.h>
#endif
#include "getos.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
char *getOSFullName(void)
{
#ifndef _MSC_VER
    struct utsname uname_pointer;
    uname(&uname_pointer);

    return strdup(uname_pointer.sysname);
#else
    return strdup("Windows");
#endif
}
/*--------------------------------------------------------------------------*/
char *getOSRelease(void)
{
#ifndef _MSC_VER
    struct utsname uname_pointer;
    uname(&uname_pointer);

    return strdup(uname_pointer.release);
#else
    switch (GetWindowsVersion())
    {
        case OS_ERROR :
        default :
            return strdup("Unknow");
            break;
        case OS_WIN32_WINDOWS_NT_3_51 :
            return strdup("NT 3.51");
            break;
        case OS_WIN32_WINDOWS_NT_4_0 :
            return strdup("NT 4.0");
            break;
        case OS_WIN32_WINDOWS_95 :
            return strdup("95");
            break;
        case OS_WIN32_WINDOWS_98 :
            return strdup("98");
            break;
        case OS_WIN32_WINDOWS_Me :
            return strdup("ME");
            break;
        case OS_WIN32_WINDOWS_2000 :
            return strdup("2000");
            break;
        case OS_WIN32_WINDOWS_XP :
            return strdup("XP");
            break;
        case OS_WIN32_WINDOWS_XP_64 :
            return strdup("XP x64");
            break;
        case OS_WIN32_WINDOWS_SERVER_2003 :
            return strdup("Server 2003");
            break;
        case OS_WIN32_WINDOWS_SERVER_2003_R2 :
            return strdup("Server 2003 R2");
            break;
        case OS_WIN32_WINDOWS_SERVER_2003_64 :
            return strdup("Server 2003 x64");
            break;
        case OS_WIN32_WINDOWS_VISTA :
            return strdup("Vista");
            break;
        case OS_WIN32_WINDOWS_VISTA_64 :
            return strdup("Vista x64");
            break;
        case OS_WIN32_WINDOWS_SERVER_2008 :
            return strdup("Server 2008");
            break;
        case OS_WIN32_WINDOWS_SERVER_2008_64 :
            return strdup("Server 2008 x64");
            break;
        case OS_WIN32_WINDOWS_SEVEN :
            return strdup("Seven");
            break;
        case OS_WIN32_WINDOWS_SEVEN_64 :
            return strdup("Seven x64");
            break;
        case OS_WIN32_WINDOWS_SEVEN_SERVER :
            return strdup("Seven Server");
            break;
        case OS_WIN32_WINDOWS_SEVEN_SERVER_64 :
            return strdup("Seven Server x64");
            break;
        case OS_WIN32_WINDOWS_EIGHT:
            return strdup("8");
            break;
        case OS_WIN32_WINDOWS_EIGHT_64:
            return strdup("8 x64");
            break;
        case OS_WIN32_WINDOWS_EIGHT_SERVER:
            return strdup("8 Server");
            break;
        case OS_WIN32_WINDOWS_EIGHT_SERVER_64:
            return strdup("8 Server x64");
            break;
    }
#endif
}
/*--------------------------------------------------------------------------*/
