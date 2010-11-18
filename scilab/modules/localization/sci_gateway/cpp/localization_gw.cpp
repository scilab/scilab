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

#include "localization_gw.hxx"

#pragma comment(lib,"../../../../bin/libintl.lib")

extern "C"
{
    #include "gw_localization.h"
}

#define MODULE_NAME L"localization"

bool LocalizationModule::Load()
{
    symbol::Context::getInstance()->AddFunction(Function::createFunction(L"gettext", &sci_gettext, MODULE_NAME));
    return true;
}
