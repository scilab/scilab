/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int sci_xdel(char *fname, unsigned long fname_len)
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
