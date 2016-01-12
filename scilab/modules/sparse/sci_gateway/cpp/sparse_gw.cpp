/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sparse_gw.hxx"
#include "context.hxx"

#define MODULE_NAME "sparse"
int SparseModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("adj2sp", &sci_adj2sp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("nnz", &sci_nnz, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sp2adj", &sci_sp2adj, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sparse", &sci_sparse, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("spcompack", &sci_spcompack, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("spget", &sci_spget, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("spzeros", &sci_spzeros, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("spones", &sci_spones, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("full", &sci_full, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ordmmd", &sci_ordmmd, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("lufact", &sci_lufact, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("lusolve", &sci_lusolve, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("luget", &sci_luget, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ludel", &sci_ludel, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("spchol", &sci_spchol, MODULE_NAME));
    return 1;
}
