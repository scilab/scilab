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

#include "configvariable.hxx"
extern "C"
{
#include "dynamic_module.h"
}

void addDynModule(const wchar_t* _pwstName, DynLibHandle _lib)
{
    ConfigVariable::addDynModule(_pwstName, _lib);
}

void removeDynModule(const wchar_t* _pwstName)
{
    ConfigVariable::removeDynModule(_pwstName);
}

DynLibHandle getDynModule(const wchar_t* _pwstName)
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
