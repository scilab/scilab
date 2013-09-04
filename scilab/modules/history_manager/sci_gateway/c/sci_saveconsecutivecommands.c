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
#include "gw_history_manager.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_saveconsecutivecommands(char *fname, unsigned long fname_len)
{
    CheckRhs(0, 1) ;
    CheckLhs(0, 1) ;

    if (Rhs == 0)
    {
        int n1 = 1, l1 = 0;
        CreateVar(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &l1);

        if ( getSaveConsecutiveDuplicateLinesInScilabHistory() )
        {
            *istk(l1) = (int)(TRUE);
        }
        else
        {
            *istk(l1) = (int)(FALSE);
        }
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        if (GetType(1) == sci_boolean)
        {
            int n1 = 0, l1 = 0, m1 = 0;
            BOOL bOK = FALSE;
            GetRhsVar(1, MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1, &l1);
            bOK = *istk(l1);
            setSaveConsecutiveDuplicateLinesInScilabHistory(bOK);
            LhsVar(1) = 0;
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 1);
            return 0;
        }
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
