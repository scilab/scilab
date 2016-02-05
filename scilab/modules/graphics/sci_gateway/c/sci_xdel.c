/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: sci_xdel.c                                                       */
/* desc : interface for xdel routine                                      */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "localization.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "FigureList.h"
#include "Scierror.h"
#include "HandleManagement.h"

#include "deleteGraphicObject.h"
#include "CurrentFigure.h"

/*--------------------------------------------------------------------------*/
int sci_xdel(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;

    int m1 = 0, n1 = 0;
    int iCurrentFigure = 0;
    CheckInputArgument(pvApiCtx, -1, 1);
    if (nbInputArgument(pvApiCtx) >= 1)
    {
        int i = 0;
        double* windowNumbers = NULL;
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
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
            return 1;
        }


        /* First check that all the window numbers are valid */
        windowNumbers = l1;
        for (i = 0; i < m1 * n1; i++)
        {
            if (!sciIsExistingFigure((int) windowNumbers[i]))
            {
                Scierror(999, "%s: Figure with figure_id %d does not exist.\n", fname, (int) windowNumbers[i]);
                return -1;
            }
        }

        for (i = 0; i < m1 * n1 ; i++)
        {
            sciDeleteWindow((int) windowNumbers[i]);
        }
    }
    else
    {
        iCurrentFigure = getCurrentFigure();
        if (iCurrentFigure != 0)
        {
            deleteGraphicObject(iCurrentFigure);
        }
    }
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
