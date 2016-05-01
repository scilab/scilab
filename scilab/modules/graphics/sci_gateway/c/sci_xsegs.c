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
/* file: sci_xsegs.c                                                      */
/* desc : interface for xsegs routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "sciCall.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "BuildObjects.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
/*--------------------------------------------------------------------------*/
int sci_xsegs(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrlx = NULL;
    double* lx = NULL;
    int* piAddrly = NULL;
    double* ly = NULL;
    int* piAddrlz = NULL;
    double* lz = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;
    int* lc = NULL;

    int color = 0;
    int *piColor = &color;
    int colorFlag;
    int* style = NULL;
    double* zptr = NULL;
    int mx = 0, nx = 0, my = 0, ny = 0, mz = 0, nz = 0, mc = 0, nc = 0;
    const double arsize = 0.0 ; // no arrow here
    int iSubwinUID = 0;

    CheckInputArgument(pvApiCtx, 2, 4);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrlx);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrlx, &mx, &nx, &lx);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrly);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrly, &my, &ny, &ly);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        return 1;
    }

    //CheckSameDims
    if (mx != my || nx != ny)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, mx, nx);
        return 1;
    }

    if (my * ny == 0)
    {
        /* Empty segs */
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (nbInputArgument(pvApiCtx) == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrlz);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        getVarDimension(pvApiCtx, piAddrlz, &mz, &nz);
        if (mz * nz == mx * nx)
        {

            // Retrieve a matrix of double at position 3.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrlz, &mz, &nz, &lz);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
                return 1;
            }

            zptr = lz;
        }
        else
        {
            mc = mz;
            nc = nz;

            if (mc * nc != 1 && mx * nx / 2 != mc * nc)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: %d, %d or %d expected.\n"), fname, 3, 1, mx * nx / 2, mx * nx);
                return 0;
            }
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 3.
            sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr3, &mc, &nc, &lc);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
                return 1;
            }

            //CheckVector
            if (mc != 1 && nc != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 3);
                return 1;
            }
        }
    }

    if (nbInputArgument(pvApiCtx) == 4)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrlz);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrlz, &mz, &nz, &lz);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
            return 1;
        }

        //CheckSameDims
        if (mx != mz || nx != nz)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, mx, nx);
            return 1;
        }

        zptr = lz;

        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 4.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr4, &mc, &nc, &lc);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
            return 1;
        }

        //CheckVector
        if (mc != 1 && nc != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 4);
            return 1;
        }


        if (mc * nc != 1 && mx * nx / 2 != mc * nc)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d or %d expected.\n"), fname, 4, 1, mx * nx / 2);
            return 0;
        }
    }

    iSubwinUID = getOrCreateDefaultSubwin();

    if (mc * nc == 0)
    {
        /* no color specified, use current color (taken from axes parent) */
        getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piColor);

        style = &color;
        colorFlag = 0;
    }
    else
    {
        style = lc;
        colorFlag = (mc * nc == 1) ? 0 : 1;
    }

    Objsegs (style, colorFlag, mx * nx, (lx), (ly), zptr, arsize);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
