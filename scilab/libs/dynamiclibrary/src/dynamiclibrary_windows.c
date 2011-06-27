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
#include "dynamiclibrary_windows.h"
#include "charEncoding.h"
#include "MALLOC.h"
/*---------------------------------------------------------------------------*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibHandle LoadDynLibraryW(wchar_t *libname)
{
	return (DynLibHandle) LoadLibraryW(libname);
}
/*---------------------------------------------------------------------------*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibHandle LoadDynLibrary(char *libname)
{
	return (DynLibHandle) LoadLibrary(libname);
}
/*---------------------------------------------------------------------------*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL BOOL FreeDynLibrary(DynLibHandle hInstance)
{
	return (BOOL) FreeLibrary((HMODULE) hInstance);
}
/*---------------------------------------------------------------------------*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle _hLib, char* _pstEntryPointName)
{
	DynLibFuncPtr retFuncPtr = NULL;
	if (_hLib)
	{
		retFuncPtr = GetProcAddress(_hLib, _pstEntryPointName);
	}
	
	return retFuncPtr;
}
/*---------------------------------------------------------------------------*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibFuncPtr GetDynLibFuncPtrW(DynLibHandle _hLib, wchar_t* _pwstEntryPointName)
{
    DynLibFuncPtr retFuncPtr = NULL;
    if (_hLib)
    {
        char* pstEntryPoint = wide_string_to_UTF8(_pwstEntryPointName);
        retFuncPtr = GetProcAddress(_hLib, pstEntryPoint);
        FREE(pstEntryPoint);
    }
    return retFuncPtr;
}
/*---------------------------------------------------------------------------*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL wchar_t* GetLastDynLibError(void)
{
	static wchar_t buffer[512];
	DWORD dw = GetLastError(); 
	DWORD source = 0;

	if (dw == 0)
	{
		wcscpy(buffer, L"Unknown Error");
	}
    else if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, &source, dw, 0,
			buffer, 512, NULL) == 0) 
	{
			wcscpy(buffer, L"Unknown Error");
	}

	return buffer;
}
/*---------------------------------------------------------------------------*/
