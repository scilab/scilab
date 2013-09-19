/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "FileBrowser.hxx"

extern "C"
{
#include "FileBrowserChDir.h"
#include "getScilabJavaVM.h"
}
using namespace org_scilab_modules_ui_data;

/*--------------------------------------------------------------------------*/
void FileBrowserChDir(char * baseDir)
{
    if (getScilabJavaVM())
    {
        FileBrowser::setBaseDir(getScilabJavaVM(), baseDir);
    }
}
