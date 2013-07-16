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
DYNAMIC_LINK_IMPEXP DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance, char *funcName);

/**
* return last dynamic linking error
* @return a string
*/
DYNAMIC_LINK_IMPEXP char * GetLastDynLibError(void);

#endif /* __DYNAMICLIBRARY_WINDOWS_H__ */

