/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
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
