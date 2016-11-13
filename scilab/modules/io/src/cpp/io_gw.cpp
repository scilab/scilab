/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#include "io_gw.hxx"
#include "context.hxx"

extern "C"
{
#include "gw_io.h"
}
#define MODULE_NAME "io"

int IoModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("genlib", &sci_genlib, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("file", &sci_file, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("host", &sci_host, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("unix", &sci_unix, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("getenv", &sci_getenv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("setenv", &sci_setenv, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("lib", &sci_gwlib, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("write", &sci_write, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction("read", &sci_read, MODULE_NAME));
	symbol::Context::getInstance()->addFunction(types::Function::createFunction("getio", &sci_getio, MODULE_NAME));
    return 1;
}
