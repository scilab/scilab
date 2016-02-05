/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

int write_sparse(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int piNbItemRow[]	= {1, 2, 1};
    int piColPos[]		= {8, 4, 7, 2};
    double pdblSReal[]	= {1, 2, 3, 4};
    double pdblSImg[]	= {4, 3, 2, 1};
    int iNbItem			= 4;

    sciErr = createComplexSparseMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, 10, iNbItem, piNbItemRow, piColPos, pdblSReal, pdblSImg);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
