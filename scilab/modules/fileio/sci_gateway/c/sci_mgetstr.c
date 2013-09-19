/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "stack-c.h"
#include "gw_fileio.h"
#include "mgetstr.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
#define ALL_FILES_DESCRIPTOR -1
/*--------------------------------------------------------------------------*/
int sci_mgetstr(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0;
    int m2 = 0, n2 = 0, l2 = 0;
    int	l3 = 0;
    int err = 0;
    int fd = ALL_FILES_DESCRIPTOR;
    int n = 1;
    int one = 1;

    Nbvars = 0;
    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if ( Rhs >= 1)
    {
        if (GetType(1) == sci_matrix)
        {
            GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
            if (m1 *n1 == 1)
            {
                n  = *istk(l1);
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: An integer expected.\n"), fname, 1);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 1);
            return 0;
        }
    }

    if ( Rhs >= 2)
    {
        if (GetType(2) == sci_matrix)
        {
            GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
            if (m2*n2 == 1)
            {
                fd = *istk(l2);
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: An integer expected.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 2);
            return 0;
        }
    }

    CreateVar(Rhs + 1, STRING_DATATYPE, &n, &one, &l3);

    C2F(mgetstr1)(&fd, cstk(l3), &n, &err);

    LhsVar(1) = Rhs + 1;
    if (err >  0)
    {
        SciError(10000);
        return 0;
    }
    else if ( err < 0)
    {
        int n5 = 0, l5 = 0;
        /* n contains now the effectively read data */
        n5 = -err - 1;
        if ( n5 < n )
        {
            CreateVar(Rhs + 2, STRING_DATATYPE, &one, &n5, &l5);
            strcpy(cstk(l5), cstk(l3));
            LhsVar(1) = Rhs + 2;
        }
    }

    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
