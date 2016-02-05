/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "sci_malloc.h"
#include "os_string.h"

/*--------------------------------------------------------------------------*/
char *getOSFullName(void)
{
#ifndef _MSC_VER
    struct utsname uname_pointer;
    uname(&uname_pointer);

    return os_strdup(uname_pointer.sysname);
#else
    return os_strdup("Windows");
#endif
}
/*--------------------------------------------------------------------------*/
char *getOSRelease(void)
{
#ifndef _MSC_VER
    struct utsname uname_pointer;
    uname(&uname_pointer);

    return os_strdup(uname_pointer.release);
#else
    switch (GetWindowsVersion())
    {
        case OS_ERROR :
        default :
            return os_strdup("Unknow");
            break;
        case OS_WIN32_WINDOWS_NT_3_51 :
            return os_strdup("NT 3.51");
            break;
        case OS_WIN32_WINDOWS_NT_4_0 :
            return os_strdup("NT 4.0");
            break;
        case OS_WIN32_WINDOWS_95 :
            return os_strdup("95");
            break;
        case OS_WIN32_WINDOWS_98 :
            return os_strdup("98");
            break;
        case OS_WIN32_WINDOWS_Me :
            return os_strdup("ME");
            break;
        case OS_WIN32_WINDOWS_2000 :
            return os_strdup("2000");
            break;
        case OS_WIN32_WINDOWS_XP :
            return os_strdup("XP");
            break;
        case OS_WIN32_WINDOWS_XP_64 :
            return os_strdup("XP x64");
            break;
        case OS_WIN32_WINDOWS_SERVER_2003 :
            return os_strdup("Server 2003");
            break;
        case OS_WIN32_WINDOWS_SERVER_2003_R2 :
            return os_strdup("Server 2003 R2");
            break;
        case OS_WIN32_WINDOWS_SERVER_2003_64 :
            return os_strdup("Server 2003 x64");
            break;
        case OS_WIN32_WINDOWS_VISTA :
            return os_strdup("Vista");
            break;
        case OS_WIN32_WINDOWS_VISTA_64 :
            return os_strdup("Vista x64");
            break;
        case OS_WIN32_WINDOWS_SERVER_2008 :
            return os_strdup("Server 2008");
            break;
        case OS_WIN32_WINDOWS_SERVER_2008_64 :
            return os_strdup("Server 2008 x64");
            break;
        case OS_WIN32_WINDOWS_SEVEN :
            return os_strdup("Seven");
            break;
        case OS_WIN32_WINDOWS_SEVEN_64 :
            return os_strdup("Seven x64");
            break;
        case OS_WIN32_WINDOWS_SEVEN_SERVER :
            return os_strdup("Seven Server");
            break;
        case OS_WIN32_WINDOWS_SEVEN_SERVER_64 :
            return os_strdup("Seven Server x64");
            break;
        case OS_WIN32_WINDOWS_EIGHT:
            return os_strdup("8");
            break;
        case OS_WIN32_WINDOWS_EIGHT_64:
            return os_strdup("8 x64");
            break;
        case OS_WIN32_WINDOWS_EIGHT_SERVER:
            return os_strdup("8 Server");
            break;
        case OS_WIN32_WINDOWS_EIGHT_SERVER_64:
            return os_strdup("8 Server x64");
            break;
    }
#endif
}
/*--------------------------------------------------------------------------*/
