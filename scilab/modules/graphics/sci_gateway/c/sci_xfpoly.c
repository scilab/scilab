/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xfpoly.c                                                     */
/* desc : interface for xfpoly routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "sciCall.h"
#include "BuildObjects.h"

#include "HandleManagement.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xfpoly(char *fname,unsigned long fname_len)
{
    char* psubwinUID = (char*)getOrCreateDefaultSubwin();
    int iStyle = 0;
    int m1 = 0, n1 = 0, l1 = 0;
    int m2 = 0, n2 = 0, l2 = 0;
    int m3 = 0, n3 = 0, l3 = 0;
    int mn1 = 0;

    long hdl = 0; /* NG */

    CheckRhs(2, 3);

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    CheckSameDims(1, 2, m1, n1, m2, n2);

    if (Rhs == 3)
    {
        GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
        CheckScalar(3, m3, n3);
        iStyle = (int) *stk(l3);
    }

    if(iStyle == 0)
    {
        int iColorMapSize = 0;
        int* piColorMapSize = &iColorMapSize;
        int iForeGround = 0;
        int* piForeGround = &iForeGround;
        char* pstParentUID = NULL;

        //get color map size
        getGraphicObjectProperty(psubwinUID, __GO_PARENT_FIGURE__, jni_int, (void**)&pstParentUID);
        getGraphicObjectProperty(pstParentUID, __GO_COLORMAP_SIZE__, jni_int, (void**)&piColorMapSize);

        //get current foreground color
        getGraphicObjectProperty(psubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piForeGround);

        if(iForeGround == -1)
        {
            iStyle = iColorMapSize + 1;
        }
        else if(iForeGround == -2)
        {
            iStyle = iColorMapSize + 2;
        }
        else
        {
            iStyle = iForeGround;
        }
    }

    Objfpoly(stk(l1), stk(l2), m1 * n1, &iStyle, &hdl, 0);

    setGraphicObjectRelationship(psubwinUID, getObjectFromHandle(hdl));

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
