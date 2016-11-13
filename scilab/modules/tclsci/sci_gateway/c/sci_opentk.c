/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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

#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_opentk(char *fname, void* pvApiCtx)
{
    Tcl_Interp *TCLinterpLocal = NULL;

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 1, 1);

    TCLinterpLocal = Tcl_CreateInterp();
    Tcl_Init(TCLinterpLocal);
    Tk_Init(TCLinterpLocal);
    TKmainWindow = Tk_MainWindow(TCLinterpLocal);
    Tk_GeometryRequest(TKmainWindow, 200, 200);
    Tk_SetWindowBackground(TKmainWindow, WhitePixelOfScreen(Tk_Screen(TKmainWindow)));

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
