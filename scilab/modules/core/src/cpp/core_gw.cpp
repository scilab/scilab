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

#include "core_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"core"
extern "C"
{
#include "gw_core.h"
}

using namespace types;

bool CoreModule::Load()
{
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"type", &sci_type, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getmodules", &sci_getmodules, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getdebuginfo", &sci_getdebuginfo, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getversion", &sci_getversion, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"clear", &sci_clear, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"banner", &sci_banner, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"quit", &sci_quit, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"exit", &sci_exit, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"exists", &sci_exists, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"isdef", &sci_isdef, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"argn", &sci_argn, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"global", &sci_global, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"clearglobal", &sci_clearglobal, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"isglobal", &sci_isglobal, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getos", &sci_getos, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"getscilabmode", &sci_getscilabmode, MODULE_NAME));
    return true;
}

