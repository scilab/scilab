/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) - 2014 - Scilab Enteprises - Antoine ELIAS
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

#ifndef __DYNAMIC_MODULE_H__
#define __DYNAMIC_MODULE_H__

#include "dynamiclibrary.h"

void addDynModule(const wchar_t* _pwstName, DynLibHandle _lib);
void removeDynModule(const wchar_t* _pwstName);
DynLibHandle getDynModule(const wchar_t* _pwstName);
int getDynModuleCount();
DynLibHandle* getAllDynModule();
void cleanDynModule();

#endif /* !__DYNAMIC_MODULE_H__ */