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
    A[0] = bar( (int) A[0]); 
    plhs[0]=prhs[0];
}  

