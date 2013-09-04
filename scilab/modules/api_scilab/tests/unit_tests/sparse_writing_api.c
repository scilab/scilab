/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

int write_sparse(char *fname, unsigned long fname_len)
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
