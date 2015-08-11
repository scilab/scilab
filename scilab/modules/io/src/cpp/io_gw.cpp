/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "io_gw.hxx"
#include "context.hxx"

extern "C"
{
#include "gw_io.h"
}
#define MODULE_NAME L"io"

using namespace types;

int IoModule::Load()
{
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"genlib", &sci_genlib, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"file", &sci_file, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"host", &sci_host, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"unix", &sci_unix, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"getenv", &sci_getenv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"setenv", &sci_setenv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"lib", &sci_lib, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"write", &sci_write, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"read", &sci_read, MODULE_NAME));
    return 1;
}
