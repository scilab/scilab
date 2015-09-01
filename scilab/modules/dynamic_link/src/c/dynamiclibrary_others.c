/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "dynamiclibrary_others.h"
#include <stdlib.h>
#include <stdio.h>
#include "machine.h"
#include "os_string.h"
#include "sci_malloc.h"
#include "localization.h"
/*---------------------------------------------------------------------------*/
#ifdef VALGRIND_ENABLE
#include "valgrind.h"
#endif
/*---------------------------------------------------------------------------*/
#ifndef NULL
#define NULL 0
#endif
/*---------------------------------------------------------------------------*/
BOOL FreeDynLibrary(DynLibHandle hInstance)
{
    if (hInstance)
    {

#ifdef VALGRIND_ENABLE
        /*
         * Accordingly to the Valgrind FAQ, using `dlclose` will clear the
         * symbol table of the loaded library.
         */
        if (RUNNING_ON_VALGRIND)
        {
            return TRUE;
        }
#endif
        if (dlclose( hInstance) == 0)
        {
            return TRUE;
        }
        else
        {
            fprintf(stderr, "Could not free library %s\n", dlerror());
        }
    }
#ifndef NDEBUG
    else
    {
        fprintf(stderr, "FreeDynLibrary: Cannot close a not-opened library.\n");
        fflush(NULL);
    }
#endif

    return FALSE;
}
/*---------------------------------------------------------------------------*/
DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance, const char *funcName)
{
    if (hInstance)
    {
        return dlsym(hInstance, funcName);
    }
    return NULL;
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
