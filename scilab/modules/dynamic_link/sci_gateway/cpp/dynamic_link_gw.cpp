/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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
