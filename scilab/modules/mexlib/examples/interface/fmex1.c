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
#include "lib/pipo.h"

void mexFunction(nlhs, plhs, nrhs, prhs)
int nlhs, nrhs;
Matrix *plhs[];
Matrix *prhs[];
{
    Matrix *ptrA;
    double *A;
    int m, n;
    if (nrhs != 1)
    {
        mexErrMsgTxt("This function requires 1 input!");
    }
    if (nlhs > 1)
    {
        mexErrMsgTxt("This function requires at most 1 output !");
    }
    ptrA = prhs[0];
    if (! mxIsNumeric(prhs[0]))
    {
        mexErrMsgTxt("First argument must be numeric matrix.");
    }
    m = mxGetM(ptrA);
    n = mxGetN(ptrA);
    A = mxGetPr(ptrA);
    A[0] = foo( (int) A[0]);
    plhs[0] = prhs[0];
}
