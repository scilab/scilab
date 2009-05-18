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

#include "elem_func.hxx"

using namespace types;

bool ElemFuncModule::Load()
{
  symbol::Context::getInstance()->AddFunction(Function::createFunction("cos", &sci_cos, "elementary_function"));
  symbol::Context::getInstance()->AddFunction(Function::createFunction("sin", &sci_sin, "elementary_function"));
  symbol::Context::getInstance()->AddFunction(Function::createFunction("blouno", &sci_blouno, "elementary_function"));
	return true;
}

Function::ReturnValue sci_blouno() {
  printf("sci_blouno() !!!\n");
 return Function::AllGood;
}
