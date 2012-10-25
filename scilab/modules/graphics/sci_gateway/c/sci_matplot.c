/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_matplot.h                                                    */
/* desc : interface for matplot routine                                   */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "DefaultCommandArg.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_matplot(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0;
    int frame_def = 8;
    int *frame = &frame_def;
    int axes_def = 1;
    int *axes = &axes_def;
    static rhs_opts opts[] =
    {
        { -1, "axesflag", -1, 0, 0, NULL},
        { -1, "frameflag", -1, 0, 0, NULL},
        { -1, "nax", -1, 0, 0, NULL},
        { -1, "rect", -1, 0, 0, NULL},
        { -1, "strf", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    char   * strf    = NULL ;
    double* rect    = NULL ;
    int    * nax     = NULL ;
    BOOL     flagNax = FALSE;

    int* piAddr1 = NULL;
    double* l1 = NULL;

    if (nbInputArgument(pvApiCtx) <= 0)
    {
        sci_demo(fname, pvApiCtx);
        return 0;
    }
    CheckInputArgument(pvApiCtx, 1, 5);

    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        ReturnArguments(pvApiCtx);
        return 0;
    }
    if (FirstOpt() < 2)
    {
        Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"),
                 fname, 1, 2);
        return(0);
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
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
        printError(&sciErr, 0);
        return 1;
    }

    if (m1 * n1 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    GetStrf(pvApiCtx, fname, 2, opts, &strf);
    GetRect(pvApiCtx, fname, 3, opts, &rect);
    GetNax(pvApiCtx, 4, opts, &nax, &flagNax);

    getOrCreateDefaultSubwin();

    if (isDefStrf(strf))
    {
        char strfl[4];

        strcpy(strfl, DEFSTRFN);

        strf = strfl;
        if (!isDefRect(rect))
        {
            strfl[1] = '7';
        }

        GetOptionalIntArg(pvApiCtx, fname, 5, "frameflag", &frame, 1, opts);
        if (frame != &frame_def)
        {
            strfl[1] = (char)(*frame + 48);
        }
        GetOptionalIntArg(pvApiCtx, fname, 5, "axesflag", &axes, 1, opts);
        if (axes != &axes_def)
        {
            strfl[2] = (char)(*axes + 48);
        }
    }

    Objmatplot((l1), &m1, &n1, strf, rect, nax, flagNax);

    /* NG end */
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
