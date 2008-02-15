
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include "mex.h"
#include "lib/pipo.h"

void mexFunction(nlhs, plhs, nrhs, prhs)
     int nlhs, nrhs;
     Matrix *plhs[]; Matrix *prhs[];
{
  Matrix *ptrA; 
    double *A;
    int m,n;
    if (nrhs != 1) mexErrMsgTxt("This function requires 1 input!");
    if (nlhs > 1) mexErrMsgTxt("This function requires at most 1 output !");
    ptrA = prhs[0];
    if (! mxIsNumeric(prhs[0])) mexErrMsgTxt("First argument must be numeric matrix.");
    m = mxGetM(ptrA);n = mxGetN(ptrA);
    A = mxGetPr(ptrA);
    A[0] = foo( (int) A[0]); 
    plhs[0]=prhs[0];
}
