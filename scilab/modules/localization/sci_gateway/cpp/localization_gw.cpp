/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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

#include "localization_gw.hxx"
#include "context.hxx"

#ifdef _MSC_VER
#pragma comment(lib,"../../../../bin/libintl.lib")
#endif

extern "C"
{
#include "gw_localization.h"
}

#define MODULE_NAME L"localization"

int LocalizationModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"gettext", &sci_gettext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"_", &sci_gettext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"_d", &sci_gettext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dgettext", &sci_gettext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"setdefaultlanguage", &sci_setdefaultlanguage, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getdefaultlanguage", &sci_getdefaultlanguage, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"setlanguage", &sci_setlanguage, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getlanguage", &sci_getlanguage, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"addlocalizationdomain", &sci_addlocalizationdomain, MODULE_NAME));
    return 1;
}
