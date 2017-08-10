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
/* file: sci_xfarcs.c                                                     */
/* desc : interface for xfarcs routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "sciCall.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "BuildObjects.h"

#include "CurrentObject.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "createGraphicObject.h"

/*--------------------------------------------------------------------------*/
int sci_xfarcs(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddr2 = NULL;
    int* l2 = NULL;

    int m1 = 0, n1 = 0;
    int m2 = 0, n2 = 0;

    long hdl = 0;
    int iCurrentSubWin = 0;

    int i = 0;

    double angle1 = 0.0;
    double angle2 = 0.0;

    CheckInputArgument(pvApiCtx, 1, 2);

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

    if (m1 != 6)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %s expected.\n"), fname, 1, "(6,n)");
        return 0;
    }

    if (nbInputArgument(pvApiCtx) == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 2.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr2, &m2, &n2, &l2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
            return 1;
        }

        //CheckVector
        if (m2 != 1 && n2 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 2);
            return 1;
        }

        if (n1 != m2 * n2)
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d and #%d.\n"), fname, 1, 2);
            return 0;
        }
    }
    else
    {
        m2 = 1;
        n2 = n1;
        sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, 2, m2, n2, &l2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        for (i = 0; i < n2; ++i)
        {
            *((int*)(l2 + i)) = i + 1;
        }
    }

    iCurrentSubWin = getOrCreateDefaultSubwin();

    for (i = 0; i < n1; ++i)
    {
        angle1 = DEG2RAD(*(l1 + (6 * i) + 4) / 64.0);
        angle2 = DEG2RAD(*(l1 + (6 * i) + 5) / 64.0);
        Objarc(&angle1, &angle2, (l1 + (6 * i)), (l1 + (6 * i) + 1),
               (l1 + (6 * i) + 2), (l1 + (6 * i) + 3), (int*)(l2 + i), (int*)(l2 + i), TRUE, FALSE, &hdl);
    }

    /** Construct Compound and make it current object **/
    {
        int o = createCompoundSeq(iCurrentSubWin, n1);
        setCurrentObject(o);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
