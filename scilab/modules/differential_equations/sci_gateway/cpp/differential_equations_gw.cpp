/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Vincent COUVERT
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

#include "differential_equations_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME "differential_equations"

int DifferentialEquationsModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ode", &sci_ode, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("impl", &sci_impl, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("dassl", &sci_dassl, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("dasrt", &sci_dasrt, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("daskr", &sci_daskr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("odedc", &sci_odedc, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("intg", &sci_intg, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("int2d", &sci_int2d, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("int3d", &sci_int3d, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("feval", &sci_feval, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("bvode", &sci_bvode, NULL, MODULE_NAME));
    return 1;
}
