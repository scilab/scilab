
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
/* file: sci_xstringb.c                                                   */
/* desc : interface for xstringb routine                                  */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sciCall.h"
#include "freeArrayOfString.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xstringb(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddrStr = NULL;
    int* piAddrl4 = NULL;
    double* l4 = NULL;
    int* piAddrl5 = NULL;
    double* l5 = NULL;
    int* piAddrl6 = NULL;
    char* l6 = NULL;

    int m1 = 0, n1 = 0, m2 = 0, n2 = 0, m3 = 0, n3 = 0, m4 = 0, n4 = 0, m5 = 0, n5 = 0, m6 = 0, n6 = 0;
    BOOL autoSize = TRUE ;
    double x = 0., y = 0., w = 0., hx = 0.;
    char **Str = NULL;
    double rect[4], angle = 0.;
    long hdlstr = 0;
    double userSize[2] ;
    int textBoxMode = 1; // 0 : off | 1 : centered | 2 : filled

    if ( nbInputArgument(pvApiCtx) <= 0 )
    {
        /* demo */
        sci_demo(fname, pvApiCtx);
        return 0 ;
    }

    CheckInputArgument(pvApiCtx, 5, 6);

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

    //CheckScalar
    if (m1 != 1 || n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 1);
        return 1;
    }

    x = *l1;
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    // YOU MUST REMOVE YOUR VARIABLE DECLARATION "int l2".
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m2, &n2, &l2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        return 1;
    }

    //CheckScalar
    if (m2 != 1 || n2 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 2);
        return 1;
    }

    y = *l2;
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrStr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of string at position 3.
    if (getAllocatedMatrixOfString(pvApiCtx, piAddrStr, &m3, &n3, &Str))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
        return 1;
    }

    if ( m3 * n3 == 0 )
    {
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        freeArrayOfString(Str, m3 * n3);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrl4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl4, &m4, &n4, &l4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    //CheckScalar
    if (m4 != 1 || n4 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 4);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    w = *l4;
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddrl5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    // Retrieve a matrix of double at position 5.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl5, &m5, &n5, &l5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    //CheckScalar
    if (m5 != 1 || n5 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 5);
        freeArrayOfString(Str, m3 * n3);
        return 1;
    }

    hx = *l5;

    if (nbInputArgument(pvApiCtx) == 6)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddrl6);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeArrayOfString(Str, m3 * n3);
            return 1;
        }

        // Retrieve a string at position 6.
        if (isScalar(pvApiCtx, piAddrl6) == 0)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 6);
            freeArrayOfString(Str, m3 * n3);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddrl6, &l6))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 6);
            freeArrayOfString(Str, m3 * n3);
            return 1;
        }

        if (strcmp(l6, "fill") == 0 )
        {
            autoSize = FALSE ;
            textBoxMode = 2;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 6, "fill");
            freeAllocatedSingleString(l6);
            freeArrayOfString(Str, m3 * n3);
            return 0;
        }

        freeAllocatedSingleString(l6);
    }

    userSize[0] = w ;
    userSize[1] = hx ;
    Objstring (Str, m3, n3, x, y, &angle, rect, autoSize, userSize, &hdlstr, textBoxMode, NULL, NULL, FALSE, TRUE, FALSE, ALIGN_CENTER);

    freeArrayOfString(Str, m3 * n3);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;

}
/*--------------------------------------------------------------------------*/
