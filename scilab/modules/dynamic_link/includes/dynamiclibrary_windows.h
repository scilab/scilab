/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#ifndef __DYNAMICLIBRARY_WINDOWS_H__
#define __DYNAMICLIBRARY_WINDOWS_H__
#ifdef _WIN32 // guard against mis-compilation

//avoid declaration on macro min/max
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#include "dynlib_dynamic_link.h"

#include "BOOL.h"

typedef HINSTANCE DynLibHandle;
typedef FARPROC DynLibFuncPtr;

/**
* Maps the specified executable module into the address space of the calling process
* @param name of dynamic library
* @return Handle to the loaded library
*/
DYNAMIC_LINK_IMPEXP DynLibHandle LoadDynLibrary(char *libname);

/**
* Maps the specified executable module into the address space of the calling process
* @param name of dynamic library (wide char)
* @return Handle to the loaded library
*/
DYNAMIC_LINK_IMPEXP DynLibHandle LoadDynLibraryW(wchar_t *libname);


/**
* Decrements the reference count of the loaded dynamic-link library
* @param Handle to the loaded library
* @return BOOL If the function succeeds, the return value is nonzero
*/
DYNAMIC_LINK_IMPEXP BOOL FreeDynLibrary(DynLibHandle hInstance);

/**
* Retrieves the address of an exported function
* @param Handle to the loaded library
* @param string that specifies the function
* @return
*/
DYNAMIC_LINK_IMPEXP DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance, const char *funcName);

/**
* return last dynamic linking error
* @return a string
*/
DYNAMIC_LINK_IMPEXP char * GetLastDynLibError(void);

typedef int(*PROC_GATEWAY) (void);

typedef enum
{
    DYN_GW_NO_ERROR = 0,
    DYN_GW_LOAD_LIBRARY_ERROR = 1,
    DYN_GW_PTR_FUNCTION_ERROR = 2,
    DYN_GW_CALL_FUNCTION_ERROR = 3

} dynamic_gateway_error_code;

typedef enum
{
    DYNLIB_NAME_FORMAT_AUTO = 0,
    DYNLIB_NAME_FORMAT_1 = 1,
    DYNLIB_NAME_FORMAT_2 = 2,
    DYNLIB_NAME_FORMAT_3 = 3
} dynlib_name_format;

#define FORMATGATEWAYLIBNAME_1 L"%ls%ls"
#define FORMATGATEWAYLIBNAME_2 L"sci%ls%ls"
#define FORMATGATEWAYLIBNAME_3 L"libsci%ls%ls"

/**
* Build name of dynamic library based on module name
* @param[in] module name
* @param[in] generated dynamic lib name type
* @return name of dynamic library
*/
DYNAMIC_LINK_IMPEXP wchar_t* buildModuleDynLibraryNameW(const wchar_t* _pwstModuleName, dynlib_name_format _iType);
DYNAMIC_LINK_IMPEXP char* buildModuleDynLibraryName(const char* _pstModuleName, dynlib_name_format _iType);

#endif /* _WIN32 // guard against mis-compilation */
#endif /* __DYNAMICLIBRARY_WINDOWS_H__ */

