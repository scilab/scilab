/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "stack-c.h"

/******************************************
 *     Example 5
 *     reading a vector in scilab internal stack using ReadMatrix
 *     (see SCIDIR/system2/readmat.f)
 *     -->link('ext5c.o','ext5c','C')
 *     -->Amatrix=[1,2,3];b=[2,3,4];
 *     -->c=call('ext5c',b,1,'d','out',[1,3],2,'d')
 *     -->c=Amatrix+2*b
 ******************************************/

int ext5c(double *b, double *c)
{
    static double a[3];
    static int k, m, n;
    ReadMatrix("Amatrix", &m, &n, a);
    /*******************************/
    /* [m,n]=size(Amatrix)  here m=1 n=3, a=Amatrix which must exist in Scilab*/
    for (k = 0; k < n; ++k)
    {
        c[k] = a[k] + b[k] * 2.;
    }
    return (0);
}
