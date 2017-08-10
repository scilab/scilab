/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
/* file: sci_champ.c                                                      */
/* desc : interface for champ (and champ1) routine                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "DefaultCommandArg.h"
#include "Champ.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_champ (char *fname, void *pvApiCtx)
{
    return sci_champ_G(fname, C2F(champ), pvApiCtx);
}
/*--------------------------------------------------------------------------*/
int sci_champ1 (char *fname, void *pvApiCtx)
{
    return sci_champ_G(fname, C2F(champ1), pvApiCtx);
}
/*--------------------------------------------------------------------------*/
int sci_champ_G(char *fname,
                int (*func) (double *, double *, double *, double *, int *, int *, char *, double *, double *, int),
                void *pvApiCtx)
{
    SciErr sciErr;
    double arfact_def = 1.0;
    double* arfact = &arfact_def;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0, m4 = 0, n4 = 0;
    static rhs_opts opts[] =
    {
        { -1, "arfact", -1, 0, 0, NULL},
        { -1, "rect", -1, 0, 0, NULL},
        { -1, "strf", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    char   * strf = NULL;
    BOOL freeStrf = FALSE;
    char strfl[4];
    double* rect = NULL;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;

    double* l1 = NULL;
    double* l2 = NULL;
    double* l3 = NULL;
    double* l4 = NULL;

    CheckInputArgument(pvApiCtx, -1, 7);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, pvApiCtx);
        return 0;
    }
    else if (nbInputArgument(pvApiCtx) < 4)
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), fname, 4);
        return 0;
    }

    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        return 0;
    }

    if (FirstOpt(pvApiCtx) < 5)
    {
        Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"), fname, 1, 5);
        return -1;
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

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &m4, &n4, &l4);
    if (sciErr.iErr)
    {
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        printError(&sciErr, 0);
        return 1;
    }

    //CheckSameDims
    if (m3 != m4 || n3 != n4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 3, m3, n3);
        return 1;
    }

    //CheckDimProp
    if (m2 * n2 != n3)
    {
        Scierror(999, _("%s: Wrong size for input arguments: Incompatible sizes.\n"), fname);
        return 1;
    }

    //CheckDimProp
    if (m1 * n1 != m3)
    {
        Scierror(999, _("%s: Wrong size for input arguments: Incompatible sizes.\n"), fname);
        return 1;
    }

    if (m3 * n3 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (get_optional_double_arg(pvApiCtx, fname, 5, "arfact", &arfact, 1, opts) == 0)
    {
        return 0;
    }
    if (get_rect_arg(pvApiCtx, fname, 6, opts, &rect) == 0)
    {
        return 0;
    }
    if (get_strf_arg(pvApiCtx, fname, 7, opts, &strf) == 0)
    {
        return 0;
    }
    freeStrf = !isDefStrf(strf);

    getOrCreateDefaultSubwin();

    if (isDefStrf(strf))
    {
        strcpy(strfl, DEFSTRFN);
        strf = strfl;
        if (!isDefRect(rect))
        {
            strf[1] = '5';
        }
    }

    (*func)((l1), (l2), (l3), (l4), &m3, &n3, strf, rect, arfact, 4L);
    if (freeStrf)
    {
        freeAllocatedSingleString(strf);
    }
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
