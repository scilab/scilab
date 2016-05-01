/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
/* file: sci_xarcs.h                                                       */
/* desc : interface for xarcs routine                                      */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "gw_graphics.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "localization.h"
#include "Scierror.h"

#include "CurrentObject.h"

#include "graphicObjectProperties.h"
#include "createGraphicObject.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xarcs(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddr2 = NULL;
    int* l2 = NULL;

    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;

    long hdl = 0;
    int i = 0, i2 = 0;
    double angle1 = 0.0;
    double angle2 = 0.0;

    int iCurrentSubWinUID = 0;
    int iCurrentSubWinForeground = 0;
    int *piCurrentSubWinForeground = &iCurrentSubWinForeground;

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

    if (strcmp(fname, "xarcs") == 0)
    {
        if (m1 != 6)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %s expected.\n"), fname, 1, "(6,n)");
            return 0;
        }
    }
    else
    {
        if (m1 != 4)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %s expected.\n"), fname, 1, "(4,n)");
            return 0;
        }
    }

    iCurrentSubWinUID = getOrCreateDefaultSubwin();

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

        if (m2 * n2 != n1)
        {
            Scierror(999, _("%s: Wrong size for arguments #%d and #%d.\n"), fname, 1, 2);
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

        getGraphicObjectProperty(iCurrentSubWinUID, __GO_LINE_COLOR__, jni_int, (void **)&piCurrentSubWinForeground);
        for (i2 = 0; i2 < n2; ++i2)
        {
            *(int*)(l2 + i2) = iCurrentSubWinForeground;
        }
    }

    for (i = 0; i < n1; ++i)
    {
        angle1 = DEG2RAD(*(l1 + (6 * i) + 4) / 64.0);
        angle2 = DEG2RAD(*(l1 + (6 * i) + 5) / 64.0);
        Objarc(&angle1, &angle2, (l1 + (6 * i)), (l1 + (6 * i) + 1),
               (l1 + (6 * i) + 2), (l1 + (6 * i) + 3), (int*)(l2 + i), NULL, FALSE, TRUE, &hdl);
    }

    /* construct Compound and make it current object */
    {
        int o = createCompoundSeq(iCurrentSubWinUID, n1);
        setCurrentObject(o);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
