/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DatatipFunctions.hxx"
#include "ScilabView.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "BOOL.h"
#include "MALLOC.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatipsetinterp(char *fname, unsigned long fname_len)
{

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);
    int nbRow = 0, nbCol = 0;
    int stkAdr = 0;
    int *addrBool = 0;
    int boolParam;
    const char* polylineUID = NULL;
    bool interpMode = false;

    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

    if (nbRow * nbCol != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A graphic handle expected.\n"), fname, 1);
        return FALSE;
    }

    polylineUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &addrBool);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return FALSE;
    }

    if (!isBooleanType(pvApiCtx, addrBool) || !checkVarDimension(pvApiCtx, addrBool, 1, 1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
        return FALSE;
    }
    getScalarBoolean(pvApiCtx, addrBool, &boolParam);
    interpMode = boolParam;

    DatatipCreate::datatipSetInterp (getScilabJavaVM(), (char*)polylineUID, interpMode);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return TRUE;

}
