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
/* file: sci_param3d.c                                                    */
/* desc : interface for param3d routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "DefaultCommandArg.h"
/*------------------------------------------------------------------------*/
int sci_param3d(char * fname, void *pvApiCtx)
{
    SciErr sciErr;
    int izcol = 0, isfac = 0;
    static double  ebox_def[6] = { 0, 1, 0, 1, 0, 1};
    double *ebox = ebox_def;
    static int iflag_def[3] = {1, 2, 4};
    int iflag[3], *ifl = NULL, ix1 = 0, one = 1;
    double  alpha_def = 35.0 , theta_def = 45.0;
    double *alpha = &alpha_def, *theta = &theta_def;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0;
    int m3n = 0, n3n = 0; /* F.Leray 19.03.04*/

    static rhs_opts opts[] =
    {
        { -1, "alpha", -1, 0, 0, NULL},
        { -1, "ebox", -1, 0, 0, NULL},
        { -1, "flag", -1, 0, 0, NULL},
        { -1, "leg", -1, 0, 0, NULL},
        { -1, "theta", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    char * labels = NULL;
    BOOL freeLabels = FALSE;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;

    double* l1 = NULL;
    double* l2 = NULL;
    double* l3 = NULL;

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, pvApiCtx);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 3, 8);

    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        ReturnArguments(pvApiCtx);
        return 0;
    }
    if (FirstOpt(pvApiCtx) < 4)
    {
        Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"), fname, 1, 4);
        return (0);
    }

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

    if (m1 * n1 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
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


    if (get_optional_double_arg(pvApiCtx, fname, 4, "theta", &theta, 1, opts) == 0)
    {
        return 0;
    }
    if (get_optional_double_arg(pvApiCtx, fname, 5, "alpha", &alpha, 1, opts) == 0)
    {
        return 0;
    }
    if (get_labels_arg(pvApiCtx, fname, 6, opts, &labels) == 0)
    {
        return 0;
    }
    freeLabels = !isDefLegend(labels);
    iflag_def[1] = 8;
    ifl = &(iflag_def[1]);
    if (get_optional_int_arg(pvApiCtx, fname, 7, "flag", &ifl, 2, opts) == 0)
    {
        if (freeLabels)
        {
            freeAllocatedSingleString(labels);
        }
        return 0;
    }
    iflag[0] = iflag_def[0];
    iflag[1] = ifl[0];
    iflag[2] = ifl[1];
    if (get_optional_double_arg(pvApiCtx, fname, 8, "ebox", &ebox, 6, opts) == 0)
    {
        if (freeLabels)
        {
            freeAllocatedSingleString(labels);
        }
        return 0;
    }

    getOrCreateDefaultSubwin();

    ix1 = m1 * n1;

    /* NG beg */
    isfac = -1;
    izcol = 0;

    Objplot3d (fname, &isfac, &izcol, (l1), (l2), (l3), (double *) NULL, &ix1, &one, theta, alpha, labels, iflag, ebox, &m1, &n1, &m2, &n2, &m3, &n3, &m3n, &n3n); /*Adding F.Leray 12.03.04 */


    /* NG end */
    if (freeLabels)
    {
        freeAllocatedSingleString(labels);
    }
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*------------------------------------------------------------------------*/
