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

#include "cacsd_gw.hxx"
#include "function.hxx"
#include "context.hxx"

#define MODULE_NAME L"cacsd"

int CacsdModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"contr", &sci_contr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dhinf", &sci_dhinf, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"hinf", &sci_hinf, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"linmeq", &sci_linmeq, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"mucomp", &sci_mucomp, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rankqr", &sci_rankqr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ricc", &sci_ricc, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sident", &sci_sident, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"sorder", &sci_sorder, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"findBD", &sci_findbd, NULL, MODULE_NAME));

    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"arl2_ius", &sci_arl2_ius, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ldiv", &sci_ldiv, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"tr_zer", &sci_tzer, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"residu", &sci_residu, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ppol", &sci_ppol, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"freq", &sci_freq, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ltitr", &sci_ltitr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"rtitr", &sci_rtitr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"ereduc", &sci_ereduc, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fstair", &sci_fstair, NULL, MODULE_NAME));

    return 1;
}
