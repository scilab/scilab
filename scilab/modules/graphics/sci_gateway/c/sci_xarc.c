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
/* file: sci_xarc.h                                                       */
/* desc : interface for xarc routine                                      */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphics.h"
#include "sciCall.h"
#include "GetProperty.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "CurrentSubwin.h"
#include "BuildObjects.h"

/*--------------------------------------------------------------------------*/
int sci_xarc(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddrl3 = NULL;
    double* l3 = NULL;
    int* piAddrl4 = NULL;
    double* l4 = NULL;
    int* piAddr5 = NULL;
    int* l5 = NULL;
    int* piAddr6 = NULL;
    int* l6 = NULL;

    int iSubwinUID = 0;
    int m1 = 0, n1 = 0;
    long hdl = 0;
    int curcolor = 0;
    int *piCurColor = &curcolor;
    double angle1 = 0.0;
    double angle2 = 0.0;


    CheckInputArgument(pvApiCtx, 6, 6);
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

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl2, &m1, &n1, &l2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        return 1;
    }

    //CheckScalar
    if (m1 != 1 || n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrl3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 3.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl3, &m1, &n1, &l3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        return 1;
    }

    //CheckScalar
    if (m1 != 1 || n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 3);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrl4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrl4, &m1, &n1, &l4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        return 1;
    }

    //CheckScalar
    if (m1 != 1 || n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 4);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 5.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr5, &m1, &n1, &l5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        return 1;
    }

    //CheckScalar
    if (m1 != 1 || n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 5);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 6.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr6, &m1, &n1, &l6);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 6);
        return 1;
    }

    //CheckScalar
    if (m1 != 1 || n1 != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 6);
        return 1;
    }


    angle1 = DEG2RAD(*(int*)(l5) / 64.0); /* convert to radian */
    angle2 = DEG2RAD(*(int*)(l6) / 64.0);

    iSubwinUID = getOrCreateDefaultSubwin();

    getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piCurColor);

    if (strcmp(fname, "xarc") == 0)
    {
        Objarc (&angle1, &angle2, (l1), (l2), (l3), (l4), &curcolor, NULL, FALSE, TRUE, &hdl);
    }
    else /* xfarc case */
    {
        Objarc (&angle1, &angle2, (l1), (l2), (l3), (l4), NULL, &curcolor, TRUE, FALSE, &hdl);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
