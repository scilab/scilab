#include <stdio.h>
#include "mex.h"

/*
 * Test function : Storing a Matrix in Scilab workspace 
 * mexPutFull uses cwritemat 
 */

void mexFunction(nlhs, plhs, nrhs, prhs)
     int nlhs, nrhs;
     mxArray *plhs[]; mxArray *prhs[];
{  
  mxArray *array_ptr;
  int m=4,n=2,it=0,i;
  double *B;
  if (nrhs != 0) mexErrMsgTxt("Invalid number of inputs!");
  array_ptr = mxCreateFull(m,n,it);
  B = mxGetPr(array_ptr);  
  for (i=0 ; i < m*n ;i++ ) B[i]= (double) i;
  mexPutFull("C",m,n,B,NULL);
  mxFree(array_ptr);
}



