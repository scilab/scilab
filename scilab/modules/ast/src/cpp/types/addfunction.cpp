/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
#include "type_traits.hxx"

extern "C"
{
#include "addfunction.h"
}

void addCStackFunction(const char* _stName, OLDGW_FUNC _pFunc, const char* _stModule)
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(_stName, _pFunc, _stModule));
}

void addCFunction(const char* _stName, GW_C_FUNC _pFunc, const char* _stModule)
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(_stName, _pFunc, _stModule));
}

void addMexFunction(const char* _stName, MEXGW_FUNC _pFunc, const char* _stModule)
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(_stName, _pFunc, _stModule));
}
