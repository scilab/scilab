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

extern "C"
{
	#include "gw_integer.h"
}

using namespace types;

bool IntegerModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction("int8", &sci_int8, "integer"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("uint8", &sci_uint8, "integer"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("int16", &sci_int16, "integer"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("uint16", &sci_uint16, "integer"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("int32", &sci_int32, "integer"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("uint32", &sci_uint32, "integer"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("int64", &sci_int64, "integer"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("uint64", &sci_uint64, "integer"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("inttype", &sci_inttype, "integer"));
	return true;
}

