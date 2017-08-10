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
/* file: sci_contour2di.h                                                 */
/* desc : interface for contour2di routine                                */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "GetCommandArg.h"
#include "Scierror.h"
#include "Contour.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
int sci_contour2di(char * fname, void* pvApiCtx)
{
    SciErr sciErr;
    int flagx = 0, nz = 10; /* default number of level curves : 10 */
    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0, m4 = 0, n4 = 0;
    double* hl1 = NULL;
    double* hl2 = NULL;
    double* znz = NULL;
    int ix4, i = 0, un = 1;

    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;

    double* l1 = NULL;
    double* l2 = NULL;
    double* l3 = NULL;
    double* l4 = NULL;
    int* l5    = NULL;

    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 2, 2);

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
    if (m3 == 1 || n3 == 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), fname, 3);
        return 1;
    }

    //CheckDimProp
    if (m1 * n1 != m3)
    {
        Scierror(999, _("%s: Wrong size for input arguments: Incompatible sizes.\n"), fname);
        return 1;
    }

    //CheckDimProp
    if (m2 * n2 != n3)
    {
        Scierror(999, _("%s: Wrong size for input arguments: Incompatible sizes.\n"), fname);
        return 1;
    }

    /*     number of level curves */
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

    if (m4 * n4 == 1)
    {
        flagx = 0;
        nz = Max(1, (int)  * (l4));
        znz = (l4);
    }
    else
    {
        flagx = 1;
        nz = m4 * n4;
        znz = (l4);
    }

    ix4 = Max(nz, 2);

    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, 5, un, ix4, &l5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    for (i = 0 ; i < ix4 ; ++i)
    {
        l5[i] = i + 1;
    }

    if (nz == 1)
    {
        l5[1] = 1;
    }

    if (C2F(contourif)(l1, l2, l3, &m3, &n3, &flagx, &nz, znz, l5) != 0)
    {
        /* Something wrong happened */
        return -1;
    }

    C2F(getconts)(&hl1, &hl2, &m1, &n1);
    if (n1 == 0)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 6, n1, n1, &hl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, 7, n1, n1, &hl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }
    else
    {
        sciErr = createMatrixOfDouble(pvApiCtx, 6, m1, n1, hl1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = createMatrixOfDouble(pvApiCtx, 7, m1, n1, hl2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }
    }
    AssignOutputVariable(pvApiCtx, 1) = 6;
    AssignOutputVariable(pvApiCtx, 2) = 7;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
