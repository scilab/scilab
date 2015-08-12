/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "stack-c.h"
#include "gw_dynamic_link.h"
/*--------------------------------------------------------------------------*/
int sci_getdynlibext(char *fname, unsigned long fname_len)
{
    static int n1 = 0, m1 = 0;
    int outIndex = 0 ;

    CheckRhs(0, 0);
    CheckLhs(1, 1);

    m1 = (int)strlen(SHARED_LIB_EXT);
    n1 = 1;

    CreateVar( Rhs + 1, STRING_DATATYPE, &m1, &n1, &outIndex);
    strcpy(cstk(outIndex), SHARED_LIB_EXT );

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
