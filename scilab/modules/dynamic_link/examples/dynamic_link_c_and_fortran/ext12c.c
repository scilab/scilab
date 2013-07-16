/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "stack-c.h"

/****************************************
 *     same example with call to GetMatrixptr
 *     param must be defined as a scilab variable
 *     exemple with a call to matptr routine
 *     -->param=[0.04,10000,3d+7];
 *     -->link('ext12c.o','ext12c','C');
 *     -->y=ode([1;0;0],0,[0.4,4],'ext12c')
 *     ***************************
 *     param entries are in stk(lp),stk(lp+1),stk(lp+2)
 *     m,n = dimensions of param = 3,1 (or 1,3 if row v.)
 *     (note that vector param not used in this example)
 ****************************************/

int ext12c(int *neq, double *t, double *y, double *ydot)
{
    static int m, n, lp;
    GetMatrixptr("param", &m, &n, &lp);
    ydot[0] = - (*stk(lp)) * y[0] + (*stk(lp + 1)) * y[1] * y[2];
    ydot[2] = (*stk(lp + 2)) * y[1] * y[1];
    ydot[1] = -ydot[0] - ydot[2];
    return 0;
}
