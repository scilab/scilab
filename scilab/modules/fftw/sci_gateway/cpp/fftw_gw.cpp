/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scialb Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "fftw_gw.hxx"
#include "context.hxx"
#include "overload.hxx"

#define MODULE_NAME "fftw"
extern "C"
{
#include "gw_fftw.h"
}

int FFTWModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("loadfftwlibrary", &sci_loadfftwlibrary, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("disposefftwlibrary", &sci_disposefftwlibrary, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fftwlibraryisloaded", &sci_fftwlibraryisloaded, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fftw", &sci_fftw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fftw_flags", &sci_fftw_flags, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("get_fftw_wisdom", &sci_get_fftw_wisdom, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("set_fftw_wisdom", &sci_set_fftw_wisdom, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("fftw_forget_wisdom", &sci_fftw_forget_wisdom, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("dct", &sci_dct, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("dst", &sci_dst, MODULE_NAME));
    return 1;
}
