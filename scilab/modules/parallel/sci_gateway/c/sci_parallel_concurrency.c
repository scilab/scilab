/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "gw_parallel.h"
#include "concurrency.h" /* prototype for concurrency query function */
#include "api_scilab.h"


/* Just a simple scilab wrapper to parallelConcurrency():
* returns non zero when called from a concurrent execution context
*/

int sci_parallel_concurrency(char *fname,unsigned long fname_len)
{
    CheckRhs(0, 0);
    CheckLhs(1, 1);

    createScalarDouble(pvApiCtx, Rhs + 1, (double)parallelConcurrency());

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
