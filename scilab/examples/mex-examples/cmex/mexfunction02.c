#include <stdio.h>
#include "mex.h"


void mexFunction(nlhs, plhs, nrhs, prhs)
     int nlhs, nrhs;
     mxArray *plhs[]; mxArray *prhs[];
{   
  int k;
  if (nlhs > nrhs) mexErrMsgTxt("Too many outputs!");
  for (k=0; k < nrhs; k++) plhs[k]=prhs[k];
}
