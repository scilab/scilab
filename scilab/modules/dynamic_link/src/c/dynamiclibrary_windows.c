/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include <stdio.h>
#include "dynamiclibrary_windows.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "machine.h"
#include "os_swprintf.h"
/*---------------------------------------------------------------------------*/
DynLibHandle LoadDynLibraryW(wchar_t *libname)
{
    return (DynLibHandle) LoadLibraryW(libname);
}
/*---------------------------------------------------------------------------*/
DynLibHandle LoadDynLibrary(char *libname)
{
    return (DynLibHandle) LoadLibrary(libname);
}
/*---------------------------------------------------------------------------*/
BOOL FreeDynLibrary(DynLibHandle hInstance)
{
    return (BOOL) FreeLibrary((HMODULE) hInstance);
}
/*---------------------------------------------------------------------------*/
DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance, char *funcName)
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
    else if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                           FORMAT_MESSAGE_IGNORE_INSERTS, &source, dw, 0,
                           buffer, 512, NULL) == 0)
    {
        strcpy(buffer, "Unknown Error");
    }

    return buffer;
}
/*---------------------------------------------------------------------------*/
wchar_t* buildModuleDynLibraryNameW(const wchar_t* _pwstModuleName, dynlib_name_format _iType)
{
    wchar_t *pwstDynlibname = NULL;
    int iLen = (int)(wcslen(_pwstModuleName) + wcslen(SHARED_LIB_EXTW)) + 1;//+1 for null
    switch (_iType)
    {
        case DYNLIB_NAME_FORMAT_AUTO:
        default:
#ifdef _MSC_VER
            iLen = iLen + (int)wcslen(FORMATGATEWAYLIBNAME_1);
#else
            iLen = iLen + (int)wcslen(FORMATGATEWAYLIBNAME_3);
#endif
            pwstDynlibname = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);
            if (iLen)
            {
                os_swprintf(pwstDynlibname, iLen, FORMATGATEWAYLIBNAME_1, _pwstModuleName, SHARED_LIB_EXTW);
            }
            break;
        case DYNLIB_NAME_FORMAT_1:
            iLen = iLen + (int)wcslen(FORMATGATEWAYLIBNAME_1) ;
            pwstDynlibname = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);
            if (pwstDynlibname)
            {
                os_swprintf(pwstDynlibname, iLen, FORMATGATEWAYLIBNAME_1, _pwstModuleName, SHARED_LIB_EXTW);
            }
            break;
        case DYNLIB_NAME_FORMAT_2:
            iLen = iLen + (int)wcslen(FORMATGATEWAYLIBNAME_2);
            pwstDynlibname = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);
            if (pwstDynlibname)
            {
                os_swprintf(pwstDynlibname, iLen, FORMATGATEWAYLIBNAME_2, _pwstModuleName, SHARED_LIB_EXTW);
            }
            break;
        case DYNLIB_NAME_FORMAT_3:
            iLen = iLen + (int)wcslen(FORMATGATEWAYLIBNAME_3);
            pwstDynlibname = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);
            if (pwstDynlibname)
            {
                os_swprintf(pwstDynlibname, iLen, FORMATGATEWAYLIBNAME_3, _pwstModuleName, SHARED_LIB_EXTW);
            }
            break;
    }
    return pwstDynlibname;
}
/*--------------------------------------------------------------------------*/
char* buildModuleDynLibraryName(const char* _pstModuleName, dynlib_name_format _iType)
{
    wchar_t* pwstModuleName = to_wide_string(_pstModuleName);
    wchar_t* pwstDynLibName = buildModuleDynLibraryNameW(pwstModuleName, _iType);
    char* pstDynLibName = wide_string_to_UTF8(pwstDynLibName);
    FREE(pwstModuleName);
    FREE(pwstDynLibName);
    return pstDynLibName;
}
/*--------------------------------------------------------------------------*/
