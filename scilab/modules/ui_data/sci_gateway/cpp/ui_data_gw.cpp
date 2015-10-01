/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "ui_data_gw.hxx"
#include "context.hxx"
#include "overload.hxx"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"

#define MODULE_NAME L"ui_data"

int UiDataModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"browsevar", &sci_browsevar, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"editvar", &sci_editvar, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"filebrowser", &sci_filebrowser, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"updatebrowsevar", &sci_updatebrowsevar, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"closeEditvar", &sci_closeEditvar, MODULE_NAME));

    return 0;
}