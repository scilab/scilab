/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
/* file: sci_xfpoly.c                                                     */
/* desc : interface for xfpoly routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciCall.h"
#include "BuildObjects.h"

#include "HandleManagement.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xfpoly(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddrl3 = NULL;
    double* l3 = NULL;

    int iSubwinUID = getOrCreateDefaultSubwin();
    int iStyle = 0;
    int m1 = 0, n1 = 0;
    int m2 = 0, n2 = 0;
    int m3 = 0, n3 = 0;

    long hdl = 0; /* NG */

    CheckInputArgument(pvApiCtx, 2, 3);

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


    if (nbInputArgument(pvApiCtx) == 3)
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

        //CheckScalar
        if (m3 != 1 || n3 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), fname, 3);
            return 1;
        }

        iStyle = (int) * (l3);
    }

    if (iStyle == 0)
    {
        int iColorMapSize = 0;
        int* piColorMapSize = &iColorMapSize;
        int iForeGround = 0;
        int* piForeGround = &iForeGround;
        int iParentUID = 0;
        int* piParentUID = &iParentUID;

        //get color map size
        getGraphicObjectProperty(iSubwinUID, __GO_PARENT_FIGURE__, jni_int, (void**)&piParentUID);
        getGraphicObjectProperty(iParentUID, __GO_COLORMAP_SIZE__, jni_int, (void**)&piColorMapSize);

        //get current foreground color
        getGraphicObjectProperty(iSubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piForeGround);

        if (iForeGround == -1)
        {
            iStyle = iColorMapSize + 1;
        }
        else if (iForeGround == -2)
        {
            iStyle = iColorMapSize + 2;
        }
        else
        {
            iStyle = iForeGround;
        }
    }

    Objfpoly((l1), (l2), m1 * n1, &iStyle, &hdl, 0);

    setGraphicObjectRelationship(iSubwinUID, getObjectFromHandle(hdl));

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
