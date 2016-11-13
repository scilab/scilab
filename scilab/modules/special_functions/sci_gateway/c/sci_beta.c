/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - Université de Nancy - Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
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

#include <string.h>
#include "api_scilab.h"
#include "gw_special_functions.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern double C2F(dgammacody)(double *);
extern double C2F(betaln)(double *, double *);
/*--------------------------------------------------------------------------*/
int sci_beta(char *fname, void* pvApiCtx)
{
    /*
    *   z = beta(x, y)
    *
    *      x, y : matrices of the same size of positive reals
    *
    *   For small x+y values uses the expression with the
    *   gamma function, else the exponential applied on the log beta
    *   function (provided in the dcd lib)
    *
    *   The switch limit have been set by using the gp-pari software.
    *
    */
    int mx = 0, nx = 0, itx = 0, lx = 0, lxc = 0, my = 0, ny = 0;
    int ity = 0, ly = 0, lyc = 0, lz = 0, i = 0;
    double *x = NULL, *y = NULL, *z = NULL, xpy = 0.;
    double switch_limit = 2;

    SciErr sciErr;

    double* pdblX = NULL;
    double* pdblY = NULL;
    double* pdblZ = NULL;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;

    int nbInputArg = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    /* get X */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddr1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &mx, &nx, &pdblX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* get Y */
    //get variable address of the input argument
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddr2))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &my, &ny, &pdblY);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (mx != my || nx != ny)
    {
        Scierror(999, _("%s: arguments #%d and #%d have incompatible dimensions.\n"), fname, 1, 2);
        return 1;
    }

    for ( i = 0 ; i < mx * nx ; i++ )
    {
        if ( (pdblX[i] <= 0.0)  ||  (pdblY[i] <= 0.0) )
        {
            Scierror(999, _("%s: Wrong value for input arguments: Must be > %d.\n"), fname, 0);
            return 1;
        }
    }

    allocMatrixOfDouble(pvApiCtx, nbInputArg + 1, mx, nx, &pdblZ);

    for ( i = 0 ; i < mx * nx ; i++ )
    {
        xpy = pdblX[i] + pdblY[i];

        if ( xpy <= switch_limit )
        {
            pdblZ[i] = C2F(dgammacody)(&pdblX[i]) * C2F(dgammacody)(&pdblY[i]) / C2F(dgammacody)(&xpy);
        }
        else
        {
            pdblZ[i] = exp(C2F(betaln)(&pdblX[i], &pdblY[i]));
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArg + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
