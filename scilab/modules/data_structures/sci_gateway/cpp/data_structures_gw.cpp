/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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

#include "data_structures_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"data_structures"

extern "C"
{
#include "gw_data_structures1.h"
#include "gw_data_structures2.h"
}

int DataStructuresModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"getfield", &sci_getfield, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"setfield", &sci_setfield, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rlist", &sci_rlist, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"definedfields", &sci_definedfields, MODULE_NAME));
    return 1;
}
