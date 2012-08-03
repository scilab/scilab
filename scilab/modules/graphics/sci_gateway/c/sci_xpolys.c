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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xpolys.c                                                     */
/* desc : interface for xpolys routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
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
    int m1 = 0, n1 = 0, l1 = 0;
    int m2 = 0, n2 = 0, l2 = 0;
    int m3 = 0, n3 = 0, l3 = 0;

    int i = 0;
    long hdl = 0;

    char *pstFigureUID = NULL;
    char *pstSubWinUID = NULL;
    char *pstCompoundUID = NULL;
    int iFalse = 0;

    int iVisible = 0;
    int *piVisible = &iVisible;

    CheckRhs(2, 3);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    CheckSameDims(1, 2, m1, n1, m2, n2);

    if (m1 * n1 == 0 || m2 * n2 == 0)
    {
        /* dimension 0, 0 polyline to draw */
        LhsVar(1) = 0;
        PutLhsVar();
        return 0;
    }
    pstSubWinUID = getOrCreateDefaultSubwin();
    pstFigureUID = getCurrentFigure();
    // Create compound.
    pstCompoundUID = createGraphicObject(__GO_COMPOUND__);
    setGraphicObjectProperty(pstCompoundUID, __GO_VISIBLE__, &iFalse, jni_bool, 1);
    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(pstSubWinUID, pstCompoundUID);

    if (Rhs == 3)
    {
        GetRhsVar(3, MATRIX_OF_INTEGER_DATATYPE, &m3, &n3, &l3);
        CheckVector(3, m3, n3);
        CheckDimProp(1, 3, m3 * n3 < n1);
        /* Construct the polylines */
        for (i = 0; i < n1; ++i)
        {
            Objpoly(stk(l1 + (i * m1)), stk(l2 + (i * m2)), m1, 0, *istk(l3 + i), &hdl);
            // Add newly created object to Compound
            setGraphicObjectRelationship(pstCompoundUID, getObjectFromHandle(hdl));
        }
    }
    else
    {
        for (i = 0; i < n1; ++i)
        {
            Objpoly(stk(l1 + (i * m1)), stk(l2 + (i * m2)), m1, 0, 1, &hdl);
            // Add newly created object to Compound
            setGraphicObjectRelationship(pstCompoundUID, getObjectFromHandle(hdl));
        }
    }

    getGraphicObjectProperty(pstFigureUID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    setGraphicObjectProperty(pstCompoundUID, __GO_VISIBLE__, &iVisible, jni_bool, 1);

    setCurrentObject(pstCompoundUID);

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
