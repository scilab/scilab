/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "preferences_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "configvariable.hxx"
#include "Scierror.h"

#define MODULE_NAME L"preferences"

int PreferencesModule::LoadDeps(const std::wstring& _functionName)
{
    if (ConfigVariable::getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999, _("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "preferences");
        return 0;
    }

    return 1;
}

int PreferencesModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"addModulePreferences", &sci_addModulePreferences, &PreferencesModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"removeModulePreferences", &sci_removeModulePreferences, &PreferencesModule::LoadDeps, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"preferences", &sci_preferences, &PreferencesModule::LoadDeps, MODULE_NAME));
    return 1;
}
