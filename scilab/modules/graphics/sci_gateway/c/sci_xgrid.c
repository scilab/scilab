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
/* file: sci_xgrid.c                                                      */
/* desc : interface for xgrid routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "HandleManagement.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"
#include "BuildObjects.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*--------------------------------------------------------------------------*/
int sci_xgrid(char *fname, unsigned long fname_len)
{
    int style = 0;              /* Default style */
    int m1 = 0, n1 = 0, l1 = 0;
    char *pstObjUID = NULL;

    CheckRhs(0, 1);

    if (Rhs == 1)
    {
        GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
        CheckScalar(1, m1, n1);
        style = (int)*stk(l1);
    }

    pstObjUID = (char*)getOrCreateDefaultSubwin();

    setGraphicObjectProperty(pstObjUID, __GO_X_AXIS_GRID_COLOR__, &style, jni_int, 1);
    setGraphicObjectProperty(pstObjUID, __GO_Y_AXIS_GRID_COLOR__, &style, jni_int, 1);
    setGraphicObjectProperty(pstObjUID, __GO_Z_AXIS_GRID_COLOR__, &style, jni_int, 1);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
