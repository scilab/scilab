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
    return true;
}

