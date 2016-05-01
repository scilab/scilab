/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

#include "jvm_gw.hxx"
#include "context.hxx"

#pragma comment(lib,"../../../../bin/libintl.lib")

extern "C"
{
#include "gw_jvm.h"
}

#define MODULE_NAME L"jvm"

int JvmModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"system_getproperty", &sci_system_getproperty, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"system_setproperty", &sci_system_setproperty, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"javaclasspath", &sci_javaclasspath, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"javalibrarypath", &sci_javalibrarypath, MODULE_NAME));
    return 1;
}
