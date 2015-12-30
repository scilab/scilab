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

#include "dynamiclibrary.h"

void addDynModule(const char* _pstName, DynLibHandle _lib);
void removeDynModule(const char* _pstName);
DynLibHandle getDynModule(const char* _pstName);
int getDynModuleCount();
DynLibHandle* getAllDynModule();
void cleanDynModule();

#endif /* !__DYNAMIC_MODULE_H__ */