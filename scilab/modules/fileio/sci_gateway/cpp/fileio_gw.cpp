/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include "fileio_gw.hxx"

#define MODULE_NAME L"fileio"

extern "C"
{
	#include "gw_fileio.h"
}

using namespace types;

bool FileioModule::Load()
{
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"cd", &sci_chdir, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"pwd", &sci_pwd, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mopen", &sci_mopen, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mclose", &sci_mclose, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mput", &sci_mput, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"mget", &sci_mget, MODULE_NAME));
    return true;
}
