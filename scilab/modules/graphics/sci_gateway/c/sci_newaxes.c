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
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_newaxes.c                                                    */
/* desc : interface for newaxes routine                                   */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "gw_graphics.h"
#include "api_scilab.h"
#include "BuildObjects.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "localization.h"
#include "HandleManagement.h"

#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"

/*--------------------------------------------------------------------------*/
int sci_newaxes(char * fname, unsigned long fname_len)
{
    SciErr sciErr;

    long long* outindex = NULL;

    int iSubwinUID = 0;
    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 0, 1);

    getOrCreateDefaultSubwin();

    if ((iSubwinUID = ConstructSubWin (getCurrentFigure())) != 0)
    {
        if (createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + 1, getHandle(iSubwinUID)))
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = 1;
        ReturnArguments(pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
