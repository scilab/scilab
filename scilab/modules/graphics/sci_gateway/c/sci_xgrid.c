/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

    int* piAddrl = NULL;
    double* l = NULL;

    int color[] = {0, 0, 0};              /* Default color */
    double thickness[] = { -1, -1, -1};
    int style[] = {3, 3, 3};
    int m = 0, n = 0, mn = 0;
    int i;
    int iObjUID = 0;

    CheckInputArgument(pvApiCtx, 0, 3);

    if (nbInputArgument(pvApiCtx) >= 1)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl, &m, &n, &l);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
            return 1;
        }

        mn = m * n;

        if (mn == 0 || mn > 3)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar or a row vector expected.\n"), fname, 1);
            return 1;
        }

        if (mn == 1)
        {
            color[0] = (int) l[0];
            color[1] = color[0];
            color[2] = color[0];
        }
        else
        {
            for (i = 0; i < mn; i++)
            {
                color[i] = (int) l[i];
            }
        }
    }

    iObjUID = getOrCreateDefaultSubwin();

    setGraphicObjectProperty(iObjUID, __GO_X_AXIS_GRID_COLOR__, color, jni_int, 1);
    setGraphicObjectProperty(iObjUID, __GO_Y_AXIS_GRID_COLOR__, color + 1, jni_int, 1);
    setGraphicObjectProperty(iObjUID, __GO_Z_AXIS_GRID_COLOR__, color + 2, jni_int, 1);

    if (nbInputArgument(pvApiCtx) >= 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 2.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl, &m, &n, &l);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
            return 1;
        }

        mn = m * n;

        if (mn == 0 || mn > 3)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar or a row vector expected.\n"), fname, 2);
            return 1;
        }

        if (mn == 1)
        {
            thickness[0] = l[0];
            thickness[1] = l[0];
            thickness[2] = l[0];
        }
        else
        {
            for (i = 0; i < mn; i++)
            {
                thickness[i] = l[i];
            }
        }

        setGraphicObjectProperty(iObjUID, __GO_X_AXIS_GRID_THICKNESS__, thickness, jni_double, 1);
        setGraphicObjectProperty(iObjUID, __GO_Y_AXIS_GRID_THICKNESS__, thickness + 1, jni_double, 1);
        setGraphicObjectProperty(iObjUID, __GO_Z_AXIS_GRID_THICKNESS__, thickness + 2, jni_double, 1);
    }

    if (nbInputArgument(pvApiCtx) == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl, &m, &n, &l);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
            return 1;
        }

        mn = m * n;

        if (mn == 0 || mn > 3)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar or a row vector expected.\n"), fname, 3);
            return 1;
        }

        if (mn == 1)
        {
            style[0] = (int) l[0];
            style[1] = style[0];
            style[2] = style[0];
        }
        else
        {
            for (i = 0; i < mn; i++)
            {
                style[i] = (int) l[i];
            }
        }

        setGraphicObjectProperty(iObjUID, __GO_X_AXIS_GRID_STYLE__, style, jni_int, 1);
        setGraphicObjectProperty(iObjUID, __GO_Y_AXIS_GRID_STYLE__, style + 1, jni_int, 1);
        setGraphicObjectProperty(iObjUID, __GO_Z_AXIS_GRID_STYLE__, style + 2, jni_int, 1);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
