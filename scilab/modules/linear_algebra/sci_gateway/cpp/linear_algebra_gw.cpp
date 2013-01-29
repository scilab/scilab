/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "context.hxx"
#include "function.hxx"
#include "linear_algebra_gw.hxx"
extern "C"
{
#include "gw_linear_algebra.h"
}

#define MODULE_NAME L"linear_algebra"

int LinearAlgebraModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"chol", &sci_chol, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"bdiag", &sci_bdiag, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"svd", &sci_svd, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rcond", &sci_rcond, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"qr", &sci_qr, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"lu", &sci_lu, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"lsq", &sci_lsq, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inv", &sci_inv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"hess", &sci_hess, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"spec", &sci_spec, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"det", &sci_det, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"balanc", &sci_balanc, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"schur", &sci_schur, MODULE_NAME));
    return true;
}
