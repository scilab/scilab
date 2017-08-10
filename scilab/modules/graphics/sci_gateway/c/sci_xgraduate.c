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
/* file: sci_xgraduate.c                                                  */
/* desc : interface for xgraduate routine                                 */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "Format.h"

/*--------------------------------------------------------------------------*/
int sci_xgraduate(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    double* lr = NULL;

    double xa = 0., xi = 0.;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, i = 0;
    int kMinr = 0, kMaxr = 0, ar = 0, np1 = 0, np2 = 0, un = 1;

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 2, 7);
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

    //CheckScalar
    if (m1 != 1 || n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        return 1;
    }

    //CheckScalar
    if (m2 != 1 || n2 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 2);
        return 1;
    }


    C2F(graduate)((l1), (l2), &xi, &xa, &np1, &np2, &kMinr, &kMaxr, &ar);

    *l1 = xi;
    *l2 = xa;

    if (nbOutputArgument(pvApiCtx) >= 3)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 3, un, un, &lr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        lr[0] = (double) np1;
    }

    if (nbOutputArgument(pvApiCtx) >= 4)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 4, un, un, &lr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        lr[0] = (double) np2;
    }

    if (nbOutputArgument(pvApiCtx) >= 5)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 5, un, un, &lr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        lr[0] = (double) kMinr;
    }

    if (nbOutputArgument(pvApiCtx) >= 6)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 6, un, un, &lr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        lr[0] = (double) kMaxr;
    }

    if (nbOutputArgument(pvApiCtx) >= 7)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 7, un, un, &lr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        lr[0] = (double) ar;
    }

    for (i = 1; i <= nbOutputArgument(pvApiCtx) ; i++)
    {
        AssignOutputVariable(pvApiCtx, i) = i;
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
