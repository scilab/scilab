/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "FileBrowser.hxx"

extern "C"
{
#include "gw_ui_data.h"
#include "getScilabJavaVM.h"
#include "api_scilab.h"
#include "scicurdir.h"
}

using namespace org_scilab_modules_ui_data;

/*--------------------------------------------------------------------------*/
int sci_filebrowser(char *fname, unsigned long fname_len)
{
    char * cwd = NULL;
    int err = 0;

    CheckRhs(0, 0);
    CheckLhs(0, 1);

    FileBrowser::openFileBrowser(getScilabJavaVM());

    cwd = scigetcwd(&err);
    if (cwd)
    {
        FileBrowser::setBaseDir(getScilabJavaVM(), cwd);
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
