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

#define MODULE_NAME L"windows_tools"
int WindowsToolsModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"win64", &sci_win64, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"winqueryreg", &sci_winqueryreg, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dos", &sci_dos, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"consolebox", &sci_consolebox, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"createGUID", &sci_createGUID, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"winopen", &sci_winopen, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"findfileassociation", &sci_findfileassociation, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mcisendstring", &sci_mcisendstring, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"istssession", &sci_istssession, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getsystemmetrics", &sci_getsystemmetrics, MODULE_NAME));
    return 1;
}
