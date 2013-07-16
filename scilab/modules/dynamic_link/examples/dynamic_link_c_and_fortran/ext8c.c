/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "stack-c.h"

/****************************************
 *     an example with  GetMatrixptr
 *     -->param=[1,2,3];
 *     -->link('ext8c.o','ext8c','C');
 *     -->y=call('ext8c','out',size(param),1,'d');
 ****************************************/

int ext8c(double *y)
{
    static int m, n, lp, i;
    GetMatrixptr("param", &m, &n, &lp);
    /* param can be changed */
    *stk(lp) = 18.0;
    /* param can be read */
    for (i = 0; i < m * n ; i++ )
    {
        y[i] = (*stk(lp + i));
    }
    return 0;
}
