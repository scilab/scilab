/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
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
/* file: sci_move.c                                                       */
/* desc : interface for move routine                                      */
/*------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "api_scilab.h"
#include "gw_graphics.h"
#include "Interaction.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "getPropertyAssignedValue.h"

/*--------------------------------------------------------------------------*/
int sci_move(char * fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl3 = NULL;
    char* l3 = NULL;
    int* piAddrl1 = NULL;
    long long* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;

    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    int nbDim = 2;
    BOOL alone = FALSE;
    int iObjUID = 0;
    double* moveVector = NULL;

    CheckInputArgument(pvApiCtx, 2, 3);
    /*  set or create a graphic window */
    if (nbInputArgument(pvApiCtx) == 3)
    {
        char * option = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        if (getAllocatedSingleString(pvApiCtx, piAddrl3, &l3))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
            return 1;
        }

        option = l3;
        if (strcmp(option, "alone") == 0)
        {
            alone = TRUE;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 3, "alone");
            freeAllocatedSingleString(l3);
            return 1;
        }

        freeAllocatedSingleString(l3);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of handle at position 1.
    sciErr = getMatrixOfHandle(pvApiCtx, piAddrl1, &m1, &n1, &l1); /* Gets the Handle passed as argument */
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
        return 1;
    }

    iObjUID = getObjectFromHandle((long int) * l1);

    if (iObjUID == 0)
    {
        Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
        return 1;
    }

    /* Get [x,y] or [x,y,z] vector */
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

    /* size of the vector, shoul dbe 2 or 3 */
    nbDim = m2 * n2;
    if (nbDim != 2 && nbDim != 3)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector %s or %s expected.\n"), fname, 3, "[x y]", "[x,y,z]");
        return 1;
    }
    moveVector = (l2);

    Objmove(iObjUID, moveVector, nbDim, alone);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
