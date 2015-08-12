/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "TCL_Global.h"
#include "gw_tclsci.h"
/*--------------------------------------------------------------------------*/
int sci_opentk(char *fname, unsigned long l)
{
    Tcl_Interp *TCLinterpLocal = NULL;

    CheckRhs(0, 0);
    CheckLhs(1, 1);

    TCLinterpLocal = Tcl_CreateInterp();
    Tcl_Init(TCLinterpLocal);
    Tk_Init(TCLinterpLocal);
    TKmainWindow = Tk_MainWindow(TCLinterpLocal);
    Tk_GeometryRequest(TKmainWindow, 200, 200);
    Tk_SetWindowBackground(TKmainWindow, WhitePixelOfScreen(Tk_Screen(TKmainWindow)));

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
