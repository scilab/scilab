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

#define MODULE_NAME "cacsd"

int CacsdModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("contr", &sci_contr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("dhinf", &sci_dhinf, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("hinf", &sci_hinf, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("linmeq", &sci_linmeq, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("mucomp", &sci_mucomp, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("rankqr", &sci_rankqr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ricc", &sci_ricc, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sident", &sci_sident, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("sorder", &sci_sorder, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("findBD", &sci_findbd, NULL, MODULE_NAME));

    symbol::Context::getInstance()->addFunction(types::Function::createFunction("arl2_ius", &sci_arl2_ius, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ldiv", &sci_ldiv, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("tr_zer", &sci_tzer, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("residu", &sci_residu, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ppol", &sci_ppol, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("freq", &sci_freq, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ltitr", &sci_ltitr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("rtitr", &sci_rtitr, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("ereduc", &sci_ereduc, NULL, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fstair", &sci_fstair, NULL, MODULE_NAME));

    return 1;
}
