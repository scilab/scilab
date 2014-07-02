/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) - 2014 - Scilab Enteprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __DYNAMIC_MODULE_H__
#define __DYNAMIC_MODULE_H__

#include "dynlib_system_env.h"
#include "dynamiclibrary.h"

EXTERN_SYSTEM_ENV void addDynModule(const wchar_t* _pwstName, DynLibHandle _lib);
EXTERN_SYSTEM_ENV void removeDynModule(const wchar_t* _pwstName);
EXTERN_SYSTEM_ENV DynLibHandle getDynModule(const wchar_t* _pwstName);
EXTERN_SYSTEM_ENV int getDynModuleCount();
EXTERN_SYSTEM_ENV DynLibHandle* getAllDynModule();
EXTERN_SYSTEM_ENV void cleanDynModule();

#endif /* !__DYNAMIC_MODULE_H__ */