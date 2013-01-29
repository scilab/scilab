/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "context.hxx"
#include "function.hxx"
#include "console_gw.hxx"

extern "C"
{
#include "gw_console.h"
}

#define MODULE_NAME L"console"

int ConsoleModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"clc", &sci_clc, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"tohome", &sci_tohome, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"lines", &sci_lines, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"prompt", &sci_prompt, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"iswaitingforinput",   &sci_iswaitingforinput, MODULE_NAME));
    return true;
}