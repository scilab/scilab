/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_move.c                                                       */
/* desc : interface for move routine                                      */
/*------------------------------------------------------------------------*/

#include <stdio.h>

#include "stack-c.h"
#include "gw_graphics.h"
#include "Interaction.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "getPropertyAssignedValue.h"

/*--------------------------------------------------------------------------*/
int sci_move( char * fname, unsigned long fname_len )
{
    int m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, l2 = 0, m3 = 0, n3 = 0, l3 = 0;
    int nbDim = 2;
    BOOL alone = FALSE;
    char* pobjUID = NULL;
    double * moveVector = NULL;

    CheckRhs(2, 3);
    /*  set or create a graphic window */
    if (Rhs == 3)
    {
        char * option = NULL;
        GetRhsVar(3, STRING_DATATYPE, &m3, &n3, &l3);
        option = getStringFromStack(l3);
        if (strcmp(option, "alone") == 0)
        {
            alone = TRUE;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 3, "alone");
            return 0;
        }
    }

    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &m1, &n1, &l1); /* Gets the Handle passed as argument */
    pobjUID = (char*)getObjectFromHandle(getHandleFromStack(l1));

    if (pobjUID == NULL)
    {
        Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
        return 0;
    }

    /* Get [x,y] or [x,y,z] vector */
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    /* size of the vector, shoul dbe 2 or 3 */
    nbDim = m2 * n2;
    if (nbDim != 2 && nbDim != 3)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Vector %s or %s expected.\n"), fname, 3, "[x y]", "[x,y,z]");
        return 0;
    }
    moveVector = stk(l2);

    Objmove(pobjUID, moveVector, nbDim, alone);

    LhsVar(1) = 0;
    PutLhsVar();

    return 1;
}
/*--------------------------------------------------------------------------*/
