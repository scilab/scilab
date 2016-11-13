/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
int sci_fire_closing_finished(char *fname, void* pvApiCtx)
{
    CheckInputArgument(pvApiCtx, 0, 0);

    int iCurrentFigure = NULL;

    /* Set current figure as parent */
    iCurrentFigure = getCurrentFigure();
    CallScilabBridge::fireClosingFinished(getScilabJavaVM(), iCurrentFigure);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
