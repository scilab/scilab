/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __DYNAMICLIBRARY_OTHERS_H__
#define __DYNAMICLIBRARY_OTHERS_H__

#include <dlfcn.h>
#include "BOOL.h" /* BOOL */

typedef void * DynLibHandle;
typedef void * DynLibFuncPtr;

/**
* Maps the specified executable module into the address space of the calling process
* @param name of dynamic library
* @return Handle to the loaded library
*/
#define LoadDynLibrary(libname) (DynLibHandle) dlopen(libname,  RTLD_NOW | RTLD_GLOBAL);

/**
* Decrements the reference count of the loaded dynamic-link library
* @param Handle to the loaded library
* @return BOOL If the function succeeds, the return value is nonzero
*/
BOOL FreeDynLibrary(DynLibHandle hInstance);

/**
* Retrieves the address of an exported function
* @param Handle to the loaded library
* @param string that specifies the function
* @return a pointer
*/
DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance, char *funcName);

/**
* return last dynamic linking error
* @return a string
*/
#define GetLastDynLibError() dlerror()

#endif /* __DYNAMICLIBRARY_OTHERS_H__ */


