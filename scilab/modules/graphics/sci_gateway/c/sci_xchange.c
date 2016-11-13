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
/* file: sci_xchange.c                                                    */
/* desc : interface for xchange routine                                   */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "PloEch.h"

#define VIEWING_RECT_SIZE 4

/*--------------------------------------------------------------------------*/
int sci_xchange(char * fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl3 = NULL;
    int* piAddr1  = NULL;
    int* piAddr2  = NULL;
    int* piAddrl1 = NULL;
    int* piAddrl2 = NULL;

    char* l3Input = NULL;
    double* l5 = NULL;

    int m1 = 0, n1 = 0, m2 = 0, n2 = 0;
    int four = VIEWING_RECT_SIZE;
    int one  = 1;
    int * xPixCoords = NULL;
    int * yPixCoords = NULL;
    double* xCoords = NULL;
    double* yCoords = NULL;
    int viewingRect[VIEWING_RECT_SIZE];

    CheckInputArgument(pvApiCtx, 3, 3);
    CheckOutputArgument(pvApiCtx, 1, 3);

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 3.
    if (getAllocatedSingleString(pvApiCtx, piAddrl3, &l3Input))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
        return 1;
    }

    /* Convert coordinates */
    if (strcmp(l3Input, "i2f") == 0)
    {
        int* l1 = NULL;
        int* l2 = NULL;
        double* l3 = NULL;
        double* l4 = NULL;
        freeAllocatedSingleString(l3Input);

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 1.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr1, &m1, &n1, &l1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
            return 1;
        }

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

        //CheckSameDims
        if (m1 != m2 || n1 != n2)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, m1, n1);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, &l3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, m1, n1, &l4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        /* Get rectangle */
        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, one, four, &l5);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }


        xPixCoords = (int*)(l1);
        yPixCoords = (int*)(l2);
        xCoords = (l3);
        yCoords = (l4);

        convertPixelCoordsToUserCoords(xPixCoords, yPixCoords, xCoords, yCoords, m1 * n1, viewingRect);
    }
    else
    {
        double* l1 = NULL;
        double* l2 = NULL;
        int* l3 = NULL;
        int* l4 = NULL;
        freeAllocatedSingleString(l3Input);

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

        //CheckSameDims
        if (m1 != m2 || n1 != n2)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, m1, n1);
            return 1;
        }


        sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, &l3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, nbInputArgument(pvApiCtx) + 2, m1, n1, &l4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        /* Get rectangle */
        sciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, one, four, &l5);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }


        xCoords = (l1);
        yCoords = (l2);
        xPixCoords = (int*)(l3);
        yPixCoords = (int*)(l4);

        convertUserCoordToPixelCoords(xCoords, yCoords, xPixCoords, yPixCoords, m1 * n1, viewingRect);
    }

    l5[0] = viewingRect[0];
    l5[1] = viewingRect[1];
    l5[2] = viewingRect[2];
    l5[3] = viewingRect[3];

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/

#undef VIEWING_RECT_SIZE

