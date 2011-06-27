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
#include "signal_gw.hxx"

extern "C"
{
    #include "gw_signal.h"
}

#define MODULE_NAME L"signal_processing"

int SignalProcessingModule::Load()
{
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"remez", &sci_remez, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"rpem", &sci_rpem, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"corr", &sci_corr, MODULE_NAME));
   // symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"ffir", &sci_ffir, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"fft", &sci_fft, MODULE_NAME));
   // symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"fiir", &sci_fiir, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"amell", &sci_amell, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"delip", &sci_delip, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(types::Function::createFunction(L"syredi", &sci_syredi, MODULE_NAME));

    return true;
}
