/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
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
/* file: sci_geom3d.c                                                     */
/* desc : interface for geom3d routine                                    */
/*------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_graphics.h"
#include "GetProperty.h"
#include "HandleManagement.h"

#include "BuildObjects.h"

int geom3d(double *x, double *y, double *z, int n);

/*--------------------------------------------------------------------------*/
int geom3d(double *x, double *y, double *z, int n)
{
    int i = 0;
    double userCoords2d[2];

    int iCurrentSubwinUID = getOrCreateDefaultSubwin();

    for (i = 0; i < n; i++)
    {
        double userCoords[3] = {x[i], y[i], z[i]};
        sciGet2dViewCoordinate(iCurrentSubwinUID, userCoords, userCoords2d);
        x[i] = userCoords2d[0];
        y[i] = userCoords2d[1];
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int sci_geom3d(char * fname, void *pvApiCtx)
{
    SciErr sciErr;
    int ix1 = 0, m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;

    double* l1 = NULL;
    double* l2 = NULL;
    double* l3 = NULL;

    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 2, 3);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &m1, &n1, &l1);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        printError(&sciErr, 0);
        return 1;
    }

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        printError(&sciErr, 0);
        return 1;
    }

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 3.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &m3, &n3, &l3);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckSameDims
    if (m1 != m2 || n1 != n2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, m1, n1);
        return 1;
    }

    //CheckSameDims
    if (m2 != m3 || n2 != n3)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 2, m2, n2);
        return 1;
    }

    if (m1 * n1 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    ix1 = m1 * n1;
    geom3d((l1), (l2), (l3), ix1);

    AssignOutputVariable(pvApiCtx, 1) = 1;
    AssignOutputVariable(pvApiCtx, 2) = 2;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
