/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 * Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ui_data_gw.hxx"
#include "ui_data.h"
#include "function.hxx"
#include "FileBrowser.hxx"

extern "C"
{
#include "Scierror.h"
#include "getScilabJavaVM.h"
#include "scicurdir.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_ui_data;
/*--------------------------------------------------------------------------*/
static const std::string fname("filebrowser");
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_filebrowser(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    char * cwd = NULL;
    int err = 0;

    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname.data(), 0);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    FileBrowser::openFileBrowser(getScilabJavaVM());

    cwd = scigetcwd(&err);
    if (cwd)
    {
        FileBrowser::setBaseDir(getScilabJavaVM(), cwd);
    }

    return types::Function::OK;
}
