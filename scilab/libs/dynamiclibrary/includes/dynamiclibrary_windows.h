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
#ifndef __DYNAMICLIBRARY_WINDOWS_H__
#define __DYNAMICLIBRARY_WINDOWS_H__

#include <Windows.h>

#include "BOOL.h"

#ifdef EXPORT_DYNAMICLIBRARY_DLL
	#define IMPORT_EXPORT_DYNAMICLIBRARY_DLL __declspec(dllexport)
#else
	#define IMPORT_EXPORT_DYNAMICLIBRARY_DLL __declspec(dllimport)
#endif

typedef HINSTANCE DynLibHandle;
typedef FARPROC DynLibFuncPtr;

/**
* Maps the specified executable module into the address space of the calling process
* @param name of dynamic library
* @return Handle to the loaded library
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibHandle LoadDynLibrary(char *libname);

/**
* Maps the specified executable module into the address space of the calling process
* @param name of dynamic library (wide char)
* @return Handle to the loaded library
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibHandle LoadDynLibraryW(wchar_t *libname);


/**
* Decrements the reference count of the loaded dynamic-link library
* @param Handle to the loaded library
* @return BOOL If the function succeeds, the return value is nonzero
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL BOOL FreeDynLibrary(DynLibHandle hInstance);

/**
* Retrieves the address of an exported function 
* @param Handle to the loaded library
* @param string that specifies the function
* @return
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle _hLib, char* _pstEntryPointName);
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibFuncPtr GetDynLibFuncPtrW(DynLibHandle _hLib, wchar_t* _pwstEntryPointName);

/**
* return last dynamic linking error 
* @return a string
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL  char* GetLastDynLibError(void);


typedef int (*PROC_GATEWAY) (void);

typedef enum { DYN_GW_NO_ERROR = 0,
DYN_GW_LOAD_LIBRARY_ERROR = 1,
DYN_GW_PTR_FUNCTION_ERROR = 2,
DYN_GW_CALL_FUNCTION_ERROR =3

} dynamic_gateway_error_code;

typedef enum { DYNLIB_NAME_FORMAT_AUTO = 0, 
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
IMPORT_EXPORT_DYNAMICLIBRARY_DLL wchar_t* buildModuleDynLibraryNameW(const wchar_t* _pwstModuleName, dynlib_name_format _iType);
IMPORT_EXPORT_DYNAMICLIBRARY_DLL char* buildModuleDynLibraryName(const char* _pstModuleName, dynlib_name_format _iType);

#endif /* __DYNAMICLIBRARY_WINDOWS_H__ */

