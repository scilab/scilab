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

#define MODULE_NAME L"sparse"
int SparseModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"adj2sp", &sci_adj2sp, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"nnz", &sci_nnz, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sp2adj", &sci_sp2adj, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sparse", &sci_sparse, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"spcompack", &sci_spcompack, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"spget", &sci_spget, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"spzeros", &sci_spzeros, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"spones", &sci_spones, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"full", &sci_full, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ordmmd", &sci_ordmmd, MODULE_NAME));
    return 1;
}
