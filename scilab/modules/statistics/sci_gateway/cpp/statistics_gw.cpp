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
#include "statistics_gw.hxx"

extern "C"
{
    #include "gw_statistics.h"
}

#define MODULE_NAME L"statistics"

int StatisticsModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdfbet", &sci_cdfbet, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdfbin", &sci_cdfbin, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdfchi", &sci_cdfchi, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdfchn", &sci_cdfchn, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdff",   &sci_cdff,   MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdffnc", &sci_cdffnc, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdfgam", &sci_cdfgam, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdfnbn", &sci_cdfnbn, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdfnor", &sci_cdfnor, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdfpoi", &sci_cdfpoi, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"cdft",   &sci_cdft,   MODULE_NAME));

    return true;
}
