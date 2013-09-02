/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
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
#include "stack-c.h"
#include "localization.h"
#include "MALLOC.h"
#include "HistoryManager.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_gethistory(char *fname, unsigned long fname_len)
{
    static int l1 = 0, m1 = 0, n1 = 0;
    CheckRhs(0, 1);
    CheckLhs(1, 1);

    if (getNumberOfLinesInScilabHistory() <= 0)
    {
        m1 = 0;
        n1 = 0;
        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }

    if (Rhs == 1)
    {
        if (GetType(1) == sci_matrix)
        {
            int N = 0;
            char *line = NULL;
            GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
            N = *istk(l1);
            line = getNthLineInScilabHistory(N);
            if (line)
            {
                n1 = 1;
                CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, (m1 = (int)strlen(line), &m1), &n1, &line);
                FREE(line);
            }
            else
            {
                m1 = 0;
                n1 = 0;
                CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
            return 0;
        }
    }
    else
    {
        char **lines = NULL;
        int nblines = 0;

        nblines = getSizeAllLinesOfScilabHistory();
        lines = getAllLinesOfScilabHistory();

        if (lines)
        {
            int m = 0, n = 0;

            m = nblines;
            n = 1;

            CreateVarFromPtr(Rhs + 1, MATRIX_OF_STRING_DATATYPE, &m, &n, lines);

            freeArrayOfString(lines, nblines);
        }
        else
        {
            m1 = 0;
            n1 = 0;
            CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
        }
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
