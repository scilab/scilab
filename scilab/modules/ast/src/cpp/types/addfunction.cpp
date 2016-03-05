/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
#include "type_traits.hxx"

extern "C"
{
#include "addfunction.h"
}

void addCStackFunction(const wchar_t* _wstName, OLDGW_FUNC _pFunc, const wchar_t* _wstModule)
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(_wstName, _pFunc, _wstModule));
}

void addCFunction(const wchar_t* _wstName, GW_C_FUNC _pFunc, const wchar_t* _wstModule)
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(_wstName, _pFunc, _wstModule));
}

void addMexFunction(const wchar_t* _wstName, MEXGW_FUNC _pFunc, const wchar_t* _wstModule)
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(_wstName, _pFunc, _wstModule));
}
