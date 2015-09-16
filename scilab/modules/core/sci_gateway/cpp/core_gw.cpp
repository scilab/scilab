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

int CoreModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"type", &sci_type, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getmodules", &sci_getmodules, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getdebuginfo", &sci_getdebuginfo, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getversion", &sci_getversion, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"clear", &sci_clear, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"banner", &sci_banner, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"quit", &sci_quit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"exit", &sci_exit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"exists", &sci_exists, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isdef", &sci_isdef, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"argn", &sci_argn, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"global", &sci_global, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"clearglobal", &sci_clearglobal, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"isglobal", &sci_isglobal, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getos", &sci_getos, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getscilabmode", &sci_getscilabmode, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"with_module", &sci_with_module, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"lasterror", &sci_lasterror, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"warning", &sci_warning, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getmd5", &sci_getmd5, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"error", &sci_error, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"pause", &sci_pause, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"abort", &sci_abort, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"who", &sci_who, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sciargs", &sci_sciargs, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"format", &sci_format, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ieee", &sci_ieee, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"typename", &sci_typename, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"funcprot", &sci_funcprot, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mode", &sci_mode, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"where", &sci_where, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getmemory", &sci_getmemory, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"errclear", &sci_errclear, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"intppty", &sci_intppty, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"newfun", &sci_newfun, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"clearfun", &sci_clearfun, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"funptr", &sci_funptr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"analyzerOptions", &sci_analyzeroptions, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"macr2tree", &sci_macr2tree, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"predef", &sci_predef, MODULE_NAME));
    return 1;
}
