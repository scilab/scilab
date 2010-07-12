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

#define MODULE_NAME L"integer"

extern "C"
{
	#include "gw_integer.h"
}

using namespace types;

bool IntegerModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"int8", &sci_int8, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uint8", &sci_uint8, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"int16", &sci_int16, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uint16", &sci_uint16, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"int32", &sci_int32, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uint32", &sci_uint32, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"int64", &sci_int64, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"uint64", &sci_uint64, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"inttype", &sci_inttype, MODULE_NAME));
	return true;
}

