/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */


/************************************
 *     (very) simple example 1
 *     -->link('ext1c.o','ext1c','C');
 *     -->a=[1,2,3];b=[4,5,6];n=3;
 *     -->c=call('ext1c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
 *     c=a+b
 ************************************/

int ext1c(int *n, double *a, double *b,  double *c)
{
    int k;
    for (k = 0; k < *n; ++k)
    {
        c[k] = a[k] + b[k];
    }
    return (0);
}

