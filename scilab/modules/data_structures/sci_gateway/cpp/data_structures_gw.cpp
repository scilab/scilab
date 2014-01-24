/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rlist",    &sci_rlist,    MODULE_NAME));
    return 1;
}
