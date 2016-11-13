/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sfinit", &sci_sfinit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("symfcti", &sci_symfcti, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("bfinit", &sci_bfinit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("inpnvi", &sci_inpnv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("blkfc1i", &sci_blkfc1i, MODULE_NAME));
    return 1;
}
