/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "ifexpr.h"
#include "stack-c.h"
#include "parserConstant.h"
/*--------------------------------------------------------------------------*/
int C2F(ifexpr)(void)
{
    int p = 0;

    if (C2F(errgst).err1 != 0)
    {
        return FALSE;
    }
    p = C2F(recu).pt;
    do
    {
        --p;
        /* the test rstk(p).eq.307.and.ids(1,p-1).ne.blank) checks if we are */
        /* evaluating the argument of a function. ids(1,p-1).ne.blank */
        /* excludes the case of a parenthesized if expression */
        if (p == 0 || (C2F(recu).rstk[p - 1] == 307 && C2F(recu).ids[(p - 1) * 6 - 6] != blank))
        {
            return FALSE;
        }
    }
    while (C2F(recu).rstk[p - 1] != 803);
    return TRUE;
}
/*--------------------------------------------------------------------------*/


