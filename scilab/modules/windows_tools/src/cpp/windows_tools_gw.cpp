/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

#include "windows_tools_gw.hxx"

extern "C"
{
#include "gw_windows_tools.h"
}

#define MODULE_NAME "windows_tools"
int WindowsToolsModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("win64", &sci_win64, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("winqueryreg", &sci_winqueryreg, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("dos", &sci_dos, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("consolebox", &sci_consolebox, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("createGUID", &sci_createGUID, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("winopen", &sci_winopen, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("findfileassociation", &sci_findfileassociation, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mcisendstring", &sci_mcisendstring, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("istssession", &sci_istssession, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getsystemmetrics", &sci_getsystemmetrics, MODULE_NAME));
    return 1;
}
