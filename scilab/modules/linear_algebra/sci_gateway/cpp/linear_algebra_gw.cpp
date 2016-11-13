/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "context.hxx"
#include "function.hxx"
#include "linear_algebra_gw.hxx"
extern "C"
{
#include "gw_linear_algebra.h"
}

#define MODULE_NAME "linear_algebra"

int LinearAlgebraModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("chol", &sci_chol, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("bdiag", &sci_bdiag, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("svd", &sci_svd, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("rcond", &sci_rcond, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("qr", &sci_qr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("lu", &sci_lu, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("lsq", &sci_lsq, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inv", &sci_inv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("hess", &sci_hess, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("spec", &sci_spec, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("det", &sci_det, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("balanc", &sci_balanc, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("schur", &sci_schur, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("norm", &sci_norm, MODULE_NAME));
    return true;
}
