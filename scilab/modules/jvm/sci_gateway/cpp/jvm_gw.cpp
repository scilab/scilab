/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"javaclasspath", &sci_javaclasspath, MODULE_NAME));
    return 1;
}
