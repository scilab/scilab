/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include "sci_getlanguage.h"
#include "setgetlanguage.h"
#include "gw_localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getlanguagealias)(char *fname, unsigned long fname_len)
{
    static int l1, n1, m1;
    char *Output = NULL;

    CheckRhs(0, 0);
    CheckLhs(0, 1);

    Output = getlanguagealias();

    if (Output)
    {
        n1 = 1;
        CreateVarFromPtr( Rhs + 1, STRING_DATATYPE, (m1 = (int)strlen(Output), &m1), &n1, &Output);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();;
        if (Output)
        {
            FREE(Output);
            Output = NULL;
        }
    }
    else
    {
        m1 = 0;
        n1 = 0;
        l1 = 0;
        CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();;
        return 0;
    }


    return 0;
}
/*--------------------------------------------------------------------------*/
