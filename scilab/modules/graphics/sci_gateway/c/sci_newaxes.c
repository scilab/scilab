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
/* file: sci_newaxes.c                                                    */
/* desc : interface for newaxes routine                                   */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "gw_graphics.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "localization.h"
#include "HandleManagement.h"

#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"

/*--------------------------------------------------------------------------*/
int sci_newaxes( char * fname, unsigned long fname_len )
{
    char *psubwinUID = NULL;
    int outindex = 0, numrow   = 1, numcol   = 1;
    CheckRhs(0, 0);
    CheckLhs(0, 1);

    getOrCreateDefaultSubwin();

    if ((psubwinUID = (char*)ConstructSubWin (getCurrentFigure())) != NULL)
    {
        CreateVar(Rhs + 1, GRAPHICAL_HANDLE_DATATYPE, &numrow, &numcol, &outindex);

        *hstk(outindex) = getHandle(psubwinUID);

        LhsVar(1) = 1;
        PutLhsVar();
    }
    else
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
