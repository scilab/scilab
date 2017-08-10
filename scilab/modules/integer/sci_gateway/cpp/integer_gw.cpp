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

#include "integer_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"integer"

extern "C"
{
#include "gw_integer.h"
}

int IntegerModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"int8", &sci_integer8, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uint8", &sci_uinteger8, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"int16", &sci_integer16, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uint16", &sci_uinteger16, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"int32", &sci_integer32, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uint32", &sci_uinteger32, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"int64", &sci_integer64, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"uint64", &sci_uinteger64, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inttype", &sci_inttype, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"double", &sci_double, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"iconvert", &sci_iconvert, MODULE_NAME));
    return 1;
}
