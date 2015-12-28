/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "dynamic_link_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"dynamic_link"

extern "C"
{
#include "gw_dynamic_link.h"
}

int DynamicLinkModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getdynlibext", &sci_getdynlibext, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"link", &sci_link, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ulink", &sci_ulink, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"c_link", &sci_c_link, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ilib_verbose", &sci_ilib_verbose, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"addinter", &sci_addinter, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"call", &sci_call, MODULE_NAME));
    return 1;
}
