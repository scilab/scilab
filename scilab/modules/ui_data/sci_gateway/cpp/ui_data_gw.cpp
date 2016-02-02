/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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