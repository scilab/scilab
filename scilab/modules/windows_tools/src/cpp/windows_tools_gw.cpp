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

#include "windows_tools_gw.hxx"

extern "C"
{
#include "gw_windows_tools.h"
}

#define MODULE_NAME L"windows_tools"
int WindowsToolsModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"win64", &sci_win64, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"winqueryreg", &sci_winqueryreg, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"dos", &sci_dos, MODULE_NAME));
    return 1;
}
