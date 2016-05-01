/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#include <stdio.h>
#include "mex.h"

#define STRING "hello world"

void ABcopy (double *a, double *b, int mn);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    mxArray *ptrB;
    double *A, *B;
    int m, n, it, strl;
    char *str;
    if (nrhs != 2)
    {
        mexErrMsgTxt("This function requires 2 inputs!");
    }
    if (nlhs > 3)
    {
        mexErrMsgTxt("This function requires at most 3 outputs!");
    }
    if (! mxIsNumeric(prhs[0]))
    {
        mexErrMsgTxt("First argument must be numeric matrix.");
    }
    if (! mxIsChar(prhs[1]))
    {
        mexErrMsgTxt("Second argument must be a string.");
    }
    m = mxGetM(prhs[0]);
    n = mxGetN(prhs[0]);
    A = mxGetPr(prhs[0]);
    it = 0;
    ptrB = mxCreateDoubleMatrix(n, m, it);
    m = mxGetM(ptrB);
    n = mxGetN(ptrB);
    B = mxGetPr(ptrB);
    ABcopy(A, B, m * n);
    plhs[0] = prhs[0];
    plhs[1] = ptrB;
    m = mxGetM(prhs[1]);
    strl = mxGetN(prhs[1]);
    str = mxCalloc(m * strl + 1, sizeof(char));
    mxGetString(prhs[1], str, m * strl);
    plhs[2] = mxCreateString(str);
}

void ABcopy(a, b, mn)
double *a;
double *b;
int mn;
{
    int i;
    for ( i = 0 ; i < mn ; i++)
    {
        b[i] = a[i] + 33.0 + i;
    }
}
