/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#include "boolean_gw.hxx"
#include "context.hxx"
#include "function.hxx"

#define MODULE_NAME L"boolean"
extern "C"
{
#include "gw_boolean.h"
}

int BooleanModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"or", &sci_or, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"and", &sci_and, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"bool2s", &sci_bool2s, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"find", &sci_find, MODULE_NAME));
    return 1;
}

