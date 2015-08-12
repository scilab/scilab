/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "CallScilabBridge.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "BOOL.h"
#include "gw_gui.h"
#include "CurrentFigure.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
int sci_fire_closing_finished(char *fname, unsigned long fname_len)
{
    CheckInputArgument(pvApiCtx, 0, 0);

    int iCurrentFigure = NULL;

    /* Set current figure as parent */
    iCurrentFigure = getCurrentFigure();
    CallScilabBridge::fireClosingFinished(getScilabJavaVM(), iCurrentFigure);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    returnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
