#include <math.h>
#include "mex.h"

static void  memcpytest __PARAMS((int n, void *x,void *y));

void mexFunction(nlhs, plhs, nrhs, prhs)
     int nlhs, nrhs;
     mxArray *plhs[]; mxArray *prhs[];
     /*  copy via memcpy */
{
  double *v; double *w;
  int m,n;
  v= mxGetPr(prhs[0]);
  m= mxGetM(prhs[0]);  n= mxGetN(prhs[0]);
  plhs[0]= mxCreateFull(m,n,0);
  w= mxGetPr(plhs[0]);
  memcpytest(m*n, v, w);
}  

static void memcpytest(n,x,y)
     void *x, *y;
     int n;
{
  memcpy(y, x, n*sizeof(double));
}

