#include "mex.h"


/*  construct a polynomial */

void mexFunction(nlhs, plhs, nrhs, prhs)
     int nlhs, nrhs;
     mxArray *plhs[]; mxArray *prhs[];
{
    mxArray *fplhs[2]; mxArray *fprhs[2];
    int mlhs,mrhs;
    if (nrhs!=2) mexErrMsgTxt("This function requires 2 inputs!");
    if (nlhs >1) mexErrMsgTxt("This function requires 1 output!");
    mlhs=1;mrhs=2;
    fprhs[0]=prhs[0];fprhs[1]=prhs[1];
    mexCallSCILAB(mlhs,fplhs,mrhs,fprhs,"poly");
    plhs[0]=fplhs[0];
}  


