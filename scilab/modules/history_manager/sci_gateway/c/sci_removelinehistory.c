/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* desc : interface for removelinehistory primitive                         */
/*--------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "HistoryManager.h"
/*--------------------------------------------------------------------------*/
int sci_removelinehistory(char *fname, unsigned long fname_len)
{
    Rhs = Max(Rhs, 0);
    CheckRhs(1, 1);
    CheckLhs(0, 1);

    if ( GetType(1) == sci_matrix )
    {
        int N = -1;
        int l1, m1 = 1, n1 = 1;
        GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
        N = *istk(l1);

        if ( (N < 0) || (N > getSizeScilabHistory()) )
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"), fname, 1, 0, getSizeScilabHistory());
            return 0;
        }
        else
        {
            deleteNthLineScilabHistory(N);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }


    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
