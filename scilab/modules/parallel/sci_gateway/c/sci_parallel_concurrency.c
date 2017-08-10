/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
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

#include "gw_parallel.h"
#include "concurrency.h" /* prototype for concurrency query function */
#include "api_scilab.h"


/* Just a simple scilab wrapper to parallelConcurrency():
* returns non zero when called from a concurrent execution context
*/

int sci_parallel_concurrency(char *fname, void* pvApiCtx)
{
    CheckRhs(0, 0);
    CheckLhs(1, 1);

    createScalarDouble(pvApiCtx, Rhs + 1, (double)parallelConcurrency());

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
