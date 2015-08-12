/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_winsid.c                                                     */
/* desc : interface for winsid routine                                    */
/*------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "gw_graphics.h"
#include "MALLOC.h"
#include "FigureList.h"
#include "Scierror.h"
#include "returnProperty.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
int sci_winsid(char *fname, unsigned long fname_len)
{
    int status = 0;
    int nbFigure = sciGetNbFigure();
    CheckInputArgument(pvApiCtx, -1, 0);

    if (nbFigure <= 0)
    {
        /* There is no figure */
        status = sciReturnEmptyMatrix(pvApiCtx);
    }
    else
    {
        int * ids = (int*)MALLOC(nbFigure * sizeof(int));
        if (ids == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        sciGetFiguresId(ids);

        status = sciReturnRowIntVector(pvApiCtx, ids, nbFigure);
        FREE(ids);
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return status;
}
/*--------------------------------------------------------------------------*/
