/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#include "gw_time.h"
#include "api_scilab.h"
#include "timer.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_timer(char *fname, void* pvApiCtx)
{
    double timerval = 0;

    Rhs = Max(0, Rhs);
    CheckLhs(0, 1);
    CheckRhs(0, 0);

    timerval = scilab_timer();

    if (timerval >= 0.)
    {
        SciErr sciErr;
        int n1 = 1;
        double * pDblReal = NULL;

        sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, n1, n1, &pDblReal);

        *pDblReal = (double)timerval;

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        Scierror(999, _("%s: An error occurred.\n"), fname);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
