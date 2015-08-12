/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "filesmanagement.h"
#include "localization.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
#define MAX_FILES 100
/*--------------------------------------------------------------------------*/
static int DoOneLhs(void);
/*--------------------------------------------------------------------------*/
/**
* maxfiles sets the soft limit for the number of scilab files allowed to open simultaneously.
* Minimum 20
* Maximum 100
* Default 20
*/
int sci_maxfiles(char *fname, unsigned long fname_len)
{
    static int l1, n1, m1;

    Rhs = Max(0, Rhs);
    CheckRhs(0, 1);
    CheckLhs(0, 1);

    if (Rhs == 0)
    {
        DoOneLhs();
    }
    else
    {
        if ( GetType(1) == sci_matrix )
        {
            GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
            if ( (m1 == 1) && (n1 == 1) )
            {
                int NewMaxFiles = 0;
                NewMaxFiles = (unsigned long) * stk(l1);
                if (NewMaxFiles > GetMaximumFileOpenedInScilab() )
                {
                    if (NewMaxFiles > MAX_FILES)
                    {
                        sciprint(_("%s: Warning: Specified maximum number of files (%d) > Absolute maximum number of files allowed (%d).\n"), fname, NewMaxFiles, MAX_FILES);
                        DoOneLhs();
                    }
                    else
                    {
                        if ( ExtendScilabFilesList(NewMaxFiles) )
                        {
                            DoOneLhs();
                        }
                        else
                        {
                            Scierror(999, _("%s: Could not extend the number of files simultaneously open in Scilab.\n"), fname, NewMaxFiles);
                            return 0;
                        }
                    }
                }
                else
                {
                    sciprint(_("%s: Warning : only extend the limit for the number of scilab's files opened simultaneously.\n"), fname);
                    DoOneLhs();
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument: Scalar expected.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument: Matrix expected.\n"), fname);
            return 0;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int DoOneLhs(void)
{
    static int n1, m1;
    int *paramoutINT = NULL;
    paramoutINT = (int*)MALLOC(sizeof(int));

    *paramoutINT = GetMaximumFileOpenedInScilab();

    n1 = 1;
    m1 = 1;
    CreateVarFromPtr(Rhs + 1, MATRIX_OF_INTEGER_DATATYPE, &n1, &m1, &paramoutINT);

    LhsVar(1) = Rhs + 1;

    if (paramoutINT)
    {
        FREE(paramoutINT);
        paramoutINT = NULL;
    }
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
