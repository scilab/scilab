/* Copyright Inria/Enpc *//*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

/************************************
 *     (very) simple example 1
 *     with a C wrapper
 *     -->link('ext1c.o','ext1cI','C');
 *     -->a=[1,2,3];b=[4,5,6];n=3;
 *     -->c=fort('ext1cI',n,a,b)
 *     c=a+b
 ************************************/

#include "stack-c.h"

int ext14cI(char *fname)
{
    int m1, n1, l1, m2, n2, l2, m3, n3, l3, n, l4;
    int minlhs = 1, minrhs = 3, maxlhs = 1, maxrhs = 3;
    Nbvars = 0;
    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;
    GetRhsVar( 1, STRING_DATATYPE, &m1, &n1, &l1);
    GetRhsVar( 2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    GetRhsVar( 3, MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
    if ( m3 * n3 != m2 * n2)
    {
        sciprint("%s :Incompatible dimensions\n", fname);
        SciError(999);
        return (0);
    }
    CreateVar( 4, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l4);
    n = m3 * n3;
    ext14c(&n, stk(l2), stk(l3), stk(l4));
    LhsVar(1) = 4;
    PutLhsVar();
    return (0);
}


int ext14c(n, a, b, c)
int *n;
double *a, *b, *c;
{
    int k;
    for (k = 0; k < *n; ++k)
    {
        c[k] = a[k] + b[k];
    }
    return (0);
}

