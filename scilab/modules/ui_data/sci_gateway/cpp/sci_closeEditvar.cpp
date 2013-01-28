/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "EditVar.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_ui_data.h"
#include "getScilabJavaVM.h"
#include "api_scilab.h"
}

using namespace org_scilab_modules_ui_data;

/*--------------------------------------------------------------------------*/
int sci_closeEditvar(char *fname, unsigned long fname_len)
{
    CheckOutputArgument(pvApiCtx, 1, 1);
    CheckInputArgument(pvApiCtx, 0, 0);

    EditVar::closeVariableEditor(getScilabJavaVM());

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
