#include <stdio.h>
#include "mex.h"

/* Test function: mexFunction() displays A=... */

void mexFunction(nlhs, plhs, nrhs, prhs)
     int nlhs, nrhs;
     mxArray *plhs[]; mxArray *prhs[];
{   
  
  mxArray *pplhs[1]; mxArray *pprhs[1];
  int nnlhs, nnrhs;
  if (nrhs != 0) mexErrMsgTxt("Invalid number of inputs!");
  nnlhs=0; nnrhs=1;
  pprhs[0]=mxCreateString("A=[1,2;3,4]");
  mexCallSCILAB(nnlhs, pplhs, nnrhs, pprhs, "disp");
}
