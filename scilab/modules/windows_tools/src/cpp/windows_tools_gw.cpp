/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
