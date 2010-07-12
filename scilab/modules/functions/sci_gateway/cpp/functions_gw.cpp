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

#include "functions_gw.hxx"

extern "C"
{
	#include "gw_functions.h"
}

#define MODULE_NAME L"functions"

using namespace types;

bool FunctionsModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"exec", &sci_exec, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"execstr", &sci_execstr, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"funclist", &sci_funclist, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"whereis", &sci_whereis, MODULE_NAME));
	return true;
}

