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

#include "jit_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"core"

using namespace types;

int JITModule::Load()
{
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"enableJIT", &sci_enableJIT, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(Function::createFunction(L"disableJIT", &sci_disableJIT, MODULE_NAME));
    return 1;
}
