/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "hdf5_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"hdf5"

extern "C"
{
	#include "gw_fileio.h"
}

using namespace types;

bool Hdf5Module::Load()
{
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"export_to_hdf5", &sci_export_to_hdf5, MODULE_NAME));
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"import_from_hdf5", &sci_import_from_hdf5, MODULE_NAME));
    return true;
}
