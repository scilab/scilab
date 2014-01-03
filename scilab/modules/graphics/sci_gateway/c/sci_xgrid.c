/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xgrid.c                                                      */
/* desc : interface for xgrid routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "HandleManagement.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"
#include "BuildObjects.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int sci_xgrid(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;

    int style = 0;              /* Default style */
    int m1 = 0, n1 = 0;
    int iObjUID = 0;

    CheckInputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 1)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
            return 1;
        }

        //CheckScalar
        if (m1 != 1 || n1 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 1);
            return 1;
        }

        style = (int)l1[0];
    }

    iObjUID = getOrCreateDefaultSubwin();

    setGraphicObjectProperty(iObjUID, __GO_X_AXIS_GRID_COLOR__, &style, jni_int, 1);
    setGraphicObjectProperty(iObjUID, __GO_Y_AXIS_GRID_COLOR__, &style, jni_int, 1);
    setGraphicObjectProperty(iObjUID, __GO_Z_AXIS_GRID_COLOR__, &style, jni_int, 1);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
