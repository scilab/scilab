/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "scilabmode.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int C2F(sci_getscilabmode)(char *fname, unsigned long fname_len)
{
    int n1 = 0, m1 = 0;
    char *output = NULL ;

    Rhs = Max(Rhs, 0);
    CheckRhs(0, 0) ;
    CheckLhs(1, 1) ;

    switch (getScilabMode())
    {
        case SCILAB_API:
        default :
            output = strdup("API");
            break;
        case SCILAB_STD:
            output = strdup("STD");
            break;
        case SCILAB_NW:
            output = strdup("NW");
            break;
        case SCILAB_NWNI:
            output = strdup("NWNI");
            break;
    }

    n1 = 1;
    m1 = (int)strlen(output);
    CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &m1, &n1, &output);
    if (output)
    {
        FREE(output);
        output = NULL;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
