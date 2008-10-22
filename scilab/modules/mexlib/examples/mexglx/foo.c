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

#define STRING "hello world"

void ABcopy (double *a,double *b,int mn);

void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[])
{
    mxArray *ptrB;
    double *A,*B;
    int m,n,it,strl;
    char *str;
    if (nrhs!=2) mexErrMsgTxt("This function requires 2 inputs!");
    if (nlhs>3) mexErrMsgTxt("This function requires at most 3 outputs!");
    if (! mxIsNumeric(prhs[0])) mexErrMsgTxt("First argument must be numeric matrix.");
    if (! mxIsChar(prhs[1])) mexErrMsgTxt("Second argument must be a string.");
    m = mxGetM(prhs[0]);n = mxGetN(prhs[0]);
    A = mxGetPr(prhs[0]);
    it=0;
    ptrB = mxCreateDoubleMatrix(n,m,it);
    m = mxGetM(ptrB);n = mxGetN(ptrB);
    B = mxGetPr(ptrB);
    ABcopy(A,B,m*n);
    plhs[0]=prhs[0];
    plhs[1]= ptrB;
    m=mxGetM(prhs[1]);
    strl=mxGetN(prhs[1]);
    str=mxCalloc(m*strl+1,sizeof(char));
    mxGetString(prhs[1],str,m*strl);
    plhs[2]=mxCreateString(str);
}

void ABcopy(a,b,mn)
     double *a; double *b;
     int mn;
{
  int i;
  for ( i=0 ; i < mn ; i++) 
    {
      b[i] = a[i] + 33.0 +i;
    }
}
