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

#include "mex.h"
#include "temptst.h"

extern N1<double> callsquare(N1<double> n1);

void mexFunction(int nlhs, mxArray  *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs < 1)
    {
        mexErrMsgTxt("Need at least one argument");
    }
    if (mxGetM(prhs[0]) == 0)
    {
        mexErrMsgTxt("First argument must have at least one entry");
    }
    double* m = mxGetPr(prhs[0]);

    N1<double> ans = callsquare(N1<double>(*m));
    mxArray* result = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(result) = ans.data();
    plhs[0] = result;
    return;
}



