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

#include "configvariable.hxx"
extern "C"
{
#include "dynamic_module.h"
}

void addDynModule(const char* _pwstName, DynLibHandle _lib)
{
    ConfigVariable::addDynModule(_pwstName, _lib);
}

void removeDynModule(const char* _pwstName)
{
    ConfigVariable::removeDynModule(_pwstName);
}

DynLibHandle getDynModule(const char* _pwstName)
{
    return ConfigVariable::getDynModule(_pwstName);
}

int getDynModuleCount()
{
    return ConfigVariable::getDynModuleCount();
}

DynLibHandle* getAllDynModule()
{
    return ConfigVariable::getAllDynModule();
}

void cleanDynModule()
{
    ConfigVariable::cleanDynModule();
}
