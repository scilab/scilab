/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
