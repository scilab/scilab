/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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

#include "polynomials_gw.hxx"
#include "context.hxx"

#define MODULE_NAME "polynomials"

int PolynomialsModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("poly", &sci_poly, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("varn", &sci_varn, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("roots", &sci_roots, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("coeff", &sci_coeff, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("degree", &sci_degree, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("pppdiv", &sci_pppdiv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("simp_mode", &sci_simpMode, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("simp", &sci_simp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sfact", &sci_sfact, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("bezout", &sci_bezout, MODULE_NAME));
    return 1;
}


