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



