/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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