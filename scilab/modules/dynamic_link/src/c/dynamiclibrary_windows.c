/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include <stdio.h>
#include "dynamiclibrary_windows.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "machine.h"
#include "os_string.h"
/*---------------------------------------------------------------------------*/
DynLibHandle LoadDynLibrary(const char *libname)
{
    return (DynLibHandle) LoadLibraryA(libname);
}
/*---------------------------------------------------------------------------*/
BOOL FreeDynLibrary(DynLibHandle hInstance)
{
    return (BOOL) FreeLibrary((HMODULE) hInstance);
}
/*---------------------------------------------------------------------------*/
DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance, const char *funcName)
{
    DynLibFuncPtr retFuncPtr = NULL ;

    if (hInstance)
    {
        retFuncPtr = GetProcAddress(hInstance, funcName);
    }

    return retFuncPtr;
}
/*---------------------------------------------------------------------------*/
char * GetLastDynLibError(void)
{
    static char buffer[512];
    DWORD dw = GetLastError();
    DWORD source = 0;

    if (dw == 0)
    {
        strcpy(buffer, "Unknown Error");
    }
    else if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM |
                           FORMAT_MESSAGE_IGNORE_INSERTS, &source, dw, 0,
                           buffer, 512, NULL) == 0)
    {
        strcpy(buffer, "Unknown Error");
    }

    return buffer;
}
/*---------------------------------------------------------------------------*/
char* buildModuleDynLibraryName(const char* _pstModuleName, dynlib_name_format _iType)
{
    char pstDynlibname[1024];
    switch (_iType)
    {
        case DYNLIB_NAME_FORMAT_AUTO:
        default:
            sprintf_s(pstDynlibname, 1024, FORMATGATEWAYLIBNAME_1, _pstModuleName, SHARED_LIB_EXT);
            break;
        case DYNLIB_NAME_FORMAT_1:
            sprintf_s(pstDynlibname, 1024, FORMATGATEWAYLIBNAME_1, _pstModuleName, SHARED_LIB_EXT);
            break;
        case DYNLIB_NAME_FORMAT_2:
            sprintf_s(pstDynlibname, 1024, FORMATGATEWAYLIBNAME_2, _pstModuleName, SHARED_LIB_EXT);
            break;
        case DYNLIB_NAME_FORMAT_3:
            sprintf_s(pstDynlibname, 1024, FORMATGATEWAYLIBNAME_3, _pstModuleName, SHARED_LIB_EXT);
            break;
    }
    return os_strdup(pstDynlibname);
}
/*--------------------------------------------------------------------------*/
