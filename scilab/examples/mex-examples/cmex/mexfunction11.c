#include "mex.h"
#include <stdio.h>

void mexFunction(nlhs, plhs, nrhs, prhs)
     int nlhs, nrhs;
     mxArray *plhs[]; mxArray *prhs[];
{   
  if (nrhs != 0) mexErrMsgTxt("Invalid number of inputs!");
  mexEvalString("A11=[1,2,3,4];");
  return ;
}

