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

#define MODULE_NAME L"io"

using namespace types;

bool IoModule::Load()
{
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"load", &sci_load, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"genlib", &sci_genlib, MODULE_NAME));
	symbol::Context::getInstance()->AddFunction(Function::createFunction(L"file", &sci_file, MODULE_NAME));
	return true;
}

