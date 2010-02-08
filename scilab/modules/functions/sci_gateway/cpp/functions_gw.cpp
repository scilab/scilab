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

using namespace types;

bool FunctionsModule::Load()
{
	symbol::Context::getInstance()->AddFunction(Function::createFunction("exec", &sci_exec, "functions"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("execstr", &sci_execstr, "functions"));
	symbol::Context::getInstance()->AddFunction(Function::createFunction("funclist", &sci_funclist, "functions"));
	return true;
}

