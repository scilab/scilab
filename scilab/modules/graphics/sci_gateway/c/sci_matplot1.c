/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_matplot1.c                                                   */
/* desc : interface for matplot1 routine                                  */
/*------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_graphics.h"
#include "BuildObjects.h"
#include "sciCall.h"

/*--------------------------------------------------------------------------*/
int sci_matplot1( char * fname, unsigned long fname_len )
{
    SciErr sciErr;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    if ( nbInputArgument(pvApiCtx) <= 0 )
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;

    double* l1 = NULL;
    double* l2 = NULL;

    CheckInputArgument(pvApiCtx, 2, 2);
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
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
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
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 2);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckLength
    if (m2 * n2 != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, 2, m2 * n2);
        return 1;
    }

    if ( m1 * n1 == 0 )
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    getOrCreateDefaultSubwin();

    /* NG beg */
    Objmatplot1 ((l1), &m1, &n1, (l2));

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
