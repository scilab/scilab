/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "localization_gw.hxx"
#include "context.hxx"

#pragma comment(lib,"../../../../bin/libintl.lib")

extern "C"
{
#include "gw_localization.h"
}

#define MODULE_NAME "localization"

int LocalizationModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("gettext", &sci_gettext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("_", &sci_gettext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("_d", &sci_gettext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("dgettext", &sci_gettext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("setdefaultlanguage", &sci_setdefaultlanguage, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getdefaultlanguage", &sci_getdefaultlanguage, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("setlanguage", &sci_setlanguage, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getlanguage", &sci_getlanguage, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("addlocalizationdomain", &sci_addlocalizationdomain, MODULE_NAME));
    return 1;
}
