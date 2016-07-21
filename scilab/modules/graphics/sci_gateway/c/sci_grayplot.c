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
/* file: sci_grayplot.c                                                   */
/* desc : interface for grayplot routine                                  */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "DefaultCommandArg.h"
#include "sciCall.h"
#include "localization.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int sci_grayplot(char *fname, void *pvApiCtx)
{
    SciErr sciErr;
    int frame_def = 8;
    int *frame = &frame_def;
    int axes_def = 1;
    int *axes = &axes_def;
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0;
    static rhs_opts opts[] =
    {
        { -1, "axesflag", -1, 0, 0, NULL},
        { -1, "frameflag", -1, 0, 0, NULL},
        { -1, "logflag", -1, 0, 0, NULL},
        { -1, "nax", -1, 0, 0, NULL},
        { -1, "rect", -1, 0, 0, NULL},
        { -1, "strf", -1, 0, 0, NULL},
        { -1, NULL, -1, 0, 0, NULL}
    };

    char   * strf    = NULL ;
    BOOL freeStrf = FALSE;
    char strfl[4];
    double* rect    = NULL ;
    int    * nax     = NULL ;
    BOOL     flagNax = FALSE;
    char* logFlags = NULL;

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
    CheckInputArgument(pvApiCtx, 3, 7);

    if (getOptionals(pvApiCtx, fname, opts) == 0)
    {
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (FirstOpt(pvApiCtx) < 4)
    {
        Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"),
                 fname, 1, 4);
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

    //CheckVector
    if (m1 != 1 && n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 1);
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

    //CheckVector
    if (m2 != 1 && n2 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 2);
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

    if (m3 * n3 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
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


    if (get_strf_arg(pvApiCtx, fname, 4, opts, &strf) == 0)
    {
        return 0;
    }
    freeStrf = !isDefStrf(strf);
    if (get_rect_arg(pvApiCtx, fname, 5, opts, &rect) == 0)
    {
        if (freeStrf)
        {
            freeAllocatedSingleString(strf);
        }
        return 0;
    }
    if (get_nax_arg(pvApiCtx, 6, opts, &nax, &flagNax)==0)
    {
        if (freeStrf)
        {
            freeAllocatedSingleString(strf);
        }
        return 0;
    }
    if (get_logflags_arg(pvApiCtx, fname, 7, opts, &logFlags) == 0)
    {
        if (freeStrf)
        {
            freeAllocatedSingleString(strf);
        }
        return 0;
    }

    getOrCreateDefaultSubwin();

    if (isDefStrf(strf))
    {
        strcpy(strfl, DEFSTRFN);

        strf = strfl;
        if (!isDefRect(rect))
        {
            strfl[1] = '7';
        }

        if (get_optional_int_arg(pvApiCtx, fname, 7, "frameflag", &frame, 1, opts) == 0)
        {
            return 0;
        }
        if (frame != &frame_def)
        {
            strfl[1] = (char)(*frame + 48);
        }
        if (get_optional_int_arg(pvApiCtx, fname, 7, "axesflag", &axes, 1, opts) == 0)
        {
            return 0;
        }
        if (axes != &axes_def)
        {
            strfl[2] = (char)(*axes + 48);
        }
    }

    Objgrayplot ((l1), (l2), (l3), &m3, &n3, strf, rect, nax, flagNax, logFlags);

    if (freeStrf)
    {
        freeAllocatedSingleString(strf);
    }
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
