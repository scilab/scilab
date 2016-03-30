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
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"lufact", &sci_lufact, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"lusolve", &sci_lusolve, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"luget", &sci_luget, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ludel", &sci_ludel, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"spchol", &sci_spchol, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sfinit", &sci_sfinit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"symfcti", &sci_symfcti, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"bfinit", &sci_bfinit, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"inpnvi", &sci_inpnv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"blkfc1i", &sci_blkfc1i, MODULE_NAME));
    return 1;
}
