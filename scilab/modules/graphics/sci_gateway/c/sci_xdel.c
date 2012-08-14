/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xdel.c                                                       */
/* desc : interface for xdel routine                                      */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "FigureList.h"
#include "Scierror.h"
#include "HandleManagement.h"

#include "deleteGraphicObject.h"
#include "CurrentFigure.h"

/*--------------------------------------------------------------------------*/
int sci_xdel(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0;
    char *pstCurrentFigure = NULL;
    CheckRhs(-1, 1);
    if (Rhs >= 1)
    {
        int i = 0;
        double * windowNumbers = NULL;
        GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);

        /* First check that all the window numbers are valid */
        windowNumbers = stk(l1);
        for (i = 0; i < m1 * n1; i++)
        {
            if (!sciIsExistingFigure((int) windowNumbers[i]))
            {
                Scierror(999, "%s: Figure with figure_id %d does not exist.\n", fname, (int) windowNumbers[i]);
                return -1;
            }
        }

        for (i = 0; i < m1 * n1 ; i++)
        {
            sciDeleteWindow( (int) windowNumbers[i] ) ;
        }
    }
    else
    {
        pstCurrentFigure = (char*)getCurrentFigure();
        if (pstCurrentFigure != NULL)
        {
            deleteGraphicObject(pstCurrentFigure);
        }
    }
    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
