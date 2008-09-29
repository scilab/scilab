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

#include "mex.h"
#include "temptst.h"

extern N1<double> callsquare(N1<double> n1);

void mexFunction(int nlhs,mxArray  *plhs[],int nrhs, const mxArray *prhs[])
{
    if (nrhs < 1)
      mexErrMsgTxt("Need at least one argument");
    if (mxGetM(prhs[0]) == 0)
      mexErrMsgTxt("First argument must have at least one entry");
    double* m = mxGetPr(prhs[0]);

    N1<double> ans = callsquare(N1<double>(*m));
    mxArray* result = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(result) = ans.data();
    plhs[0] = result;
    return;
}



