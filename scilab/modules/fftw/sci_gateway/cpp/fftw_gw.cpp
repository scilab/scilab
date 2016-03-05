/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scialb Enterprises - Antoine ELIAS
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

#include "fftw_gw.hxx"
#include "context.hxx"
#include "overload.hxx"

#define MODULE_NAME L"fftw"
extern "C"
{
#include "gw_fftw.h"
}

int FFTWModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"loadfftwlibrary", &sci_loadfftwlibrary, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"disposefftwlibrary", &sci_disposefftwlibrary, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fftwlibraryisloaded", &sci_fftwlibraryisloaded, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fftw", &sci_fftw, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fftw_flags", &sci_fftw_flags, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"get_fftw_wisdom", &sci_get_fftw_wisdom, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"set_fftw_wisdom", &sci_set_fftw_wisdom, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fftw_forget_wisdom", &sci_fftw_forget_wisdom, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dct", &sci_dct, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dst", &sci_dst, MODULE_NAME));
    return 1;
}
