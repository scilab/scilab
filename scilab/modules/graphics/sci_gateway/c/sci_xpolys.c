/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xpolys.c                                                     */
/* desc : interface for xpolys routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciCall.h"
#include "BuildObjects.h"

#include "CurrentFigure.h"
#include "CurrentObject.h"
#include "HandleManagement.h"
#include "createGraphicObject.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xpolys(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* l1 = NULL;
    int* piAddrl2 = NULL;
    double* l2 = NULL;
    int* piAddr3 = NULL;
    int* l3 = NULL;

    int m1 = 0, n1 = 0;
    int m2 = 0, n2 = 0;
    int m3 = 0, n3 = 0;

    int i = 0;
    long hdl = 0;

    char *pstFigureUID = NULL;
    char *pstSubWinUID = NULL;
    char *pstCompoundUID = NULL;
    int iFalse = 0;

    int iVisible = 0;
    int *piVisible = &iVisible;

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
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
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
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 2);
        return 1;
    }

    //CheckSameDims
    if (m1 != m2 || n1 != n2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), fname, 1, m1, n1);
        return 1;
    }


    if (m1 * n1 == 0 || m2 * n2 == 0)
    {
        /* dimension 0, 0 polyline to draw */
        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
        return 0;
    }
    pstSubWinUID = (char*)getOrCreateDefaultSubwin();
    pstFigureUID = (char*)getCurrentFigure();
    // Create compound.
    pstCompoundUID = createGraphicObject(__GO_COMPOUND__);
    setGraphicObjectProperty(pstCompoundUID, __GO_VISIBLE__, &iFalse, jni_bool, 1);
    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(pstSubWinUID, pstCompoundUID);

    if (nbInputArgument(pvApiCtx) == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 3.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddr3, &m3, &n3, &l3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 3);
            return 1;
        }

        //CheckVector
        if (m3 != 1 && n3 != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 3);
            return 1;
        }

        //CheckDimProp
        if (m3 * n3 < n1)
        {
            Scierror(999, _("%s: Wrong size for input arguments: Incompatible sizes.\n"), fname);
            return 1;
        }

        /* Construct the polylines */
        for (i = 0; i < n1; ++i)
        {
            Objpoly((l1 + (i * m1)), (l2 + (i * m2)), m1, 0, *(int*)(l3 + i), &hdl);
            // Add newly created object to Compound
            setGraphicObjectRelationship(pstCompoundUID, getObjectFromHandle(hdl));
        }
    }
    else
    {
        for (i = 0; i < n1; ++i)
        {
            Objpoly((l1 + (i * m1)), (l2 + (i * m2)), m1, 0, 1, &hdl);
            // Add newly created object to Compound
            setGraphicObjectRelationship(pstCompoundUID, getObjectFromHandle(hdl));
        }
    }

    getGraphicObjectProperty(pstFigureUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    setGraphicObjectProperty(pstCompoundUID, __GO_VISIBLE__, &iVisible, jni_bool, 1);

    setCurrentObject(pstCompoundUID);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
