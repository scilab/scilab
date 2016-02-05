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
/* file: sci_xarrows.h                                                    */
/* desc : interface for xarrows routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "sciCall.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "BuildObjects.h"
/*--------------------------------------------------------------------------*/
int sci_xarrows(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddrl3 = NULL;
    double* l3 = NULL;
    int* piAddr4 = NULL;
    int* l4 = NULL;
    double* dl4 = NULL;

    int dstyle = -1, m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 1, n3 = 1;
    int *style = NULL, flag = 0;
    int m4 = 0, n4 = 0, mn2 = 0;
    double arsize = -1.0;
    double * zptr = NULL;
    int stylePos = 4;
    int *piAddr = NULL;

    CheckInputArgument(pvApiCtx, 2, 5);

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

    mn2 = m2 * n2;
    //CheckSameDims
    if (m1 != m2 || n1 != n2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, m1, n1);
        return 1;
    }

    if (mn2 == 0)
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (nbInputArgument(pvApiCtx) >= 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrl3, &m3, &n3, &l3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
            return 1;
        }

        if (m3 == m1 && n3 == n1)
        {
            zptr = l3;
            if (nbInputArgument(pvApiCtx) >= 4)
            {
                sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                // Retrieve a matrix of double at position 4.
                sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &m4, &n4, &dl4);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
                    return 1;
                }

                if (m4 != 1 || n4 != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 4);
                    return 1;
                }

                arsize = *dl4;
                stylePos = 5;
            }
        }
        else if (m3 == 1 && n3 == 1)
        {
            if (nbInputArgument(pvApiCtx) > 4)
            {
                Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), fname, 4);
                return 1;
            }

            arsize = *l3;
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix or a scalar expected.\n"), fname, 3, m1, n1);
            return 1;
        }
    }

    if (nbInputArgument(pvApiCtx) >= stylePos)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, stylePos, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 4 or 5.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr, &m4, &n4, &l4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, stylePos);
            return 1;
        }

        //CheckVector
        if (m4 != 1 && n4 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, stylePos);
            return 1;
        }

        if (m4 * n4 == 1)
        {
            dstyle = *(int*)(l4);
        }
        if (m4 * n4 != 1 && m2 * n2 / 2 != m4 * n4)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), fname, stylePos, m2 * n2 / 2);
            return 0;
        }
    }

    /* NG beg */
    if (nbInputArgument(pvApiCtx) == stylePos && m4 * n4 != 1)
    {
        style = (int*)(l4);
        flag = 1;
    }
    else
    {
        style = &dstyle;
        flag = 0;
    }

    getOrCreateDefaultSubwin();

    Objsegs(style, flag, mn2, (l1), (l2), zptr, arsize);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
