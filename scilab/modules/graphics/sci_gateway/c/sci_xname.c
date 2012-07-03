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
/* file: sci_xname.c                                                      */
/* desc : interface for xname routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"

#include "BuildObjects.h"
#include "CurrentFigure.h"

#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xname(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0;

    char *pstCurrentFigure = NULL;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);

    pstCurrentFigure = (char*)getCurrentFigure();

    if (pstCurrentFigure == NULL)
    {
        pstCurrentFigure = createNewFigureWithAxes();
    }

    setGraphicObjectProperty(pstCurrentFigure, __GO_NAME__, cstk(l1), jni_string, 1);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
