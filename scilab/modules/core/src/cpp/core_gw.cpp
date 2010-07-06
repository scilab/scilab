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

extern "C"
{
#include "gw_core.h"
}

using namespace types;

bool CoreModule::Load()
{
    symbol::Context::getInstance()->AddFunction(Function::createFunction("type", &sci_type, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("getmodules", &sci_getmodules, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("getdebuginfo", &sci_getdebuginfo, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("getversion", &sci_getversion, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("clear", &sci_clear, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("banner", &sci_banner, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("quit", &sci_quit, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("exit", &sci_exit, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("exists", &sci_exists, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("isdef", &sci_isdef, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("argn", &sci_argn, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("global", &sci_global, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("clearglobal", &sci_clearglobal, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("isglobal", &sci_isglobal, "core"));
    symbol::Context::getInstance()->AddFunction(Function::createFunction("getscilabmode", &sci_getscilabmode, "core"));
    return true;
}

