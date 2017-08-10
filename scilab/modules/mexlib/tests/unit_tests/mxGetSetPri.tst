// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetPr, mxGetPi, mSGetPr, mxSetPi,  mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   int i = 0;";
"   int iSize = 0;";
"   mxArray* pOut = NULL;";
"   double* pDblR = NULL;";
"   double* pDblI = NULL;";
"";
"   if(nrhs != 1 /*|| !mxIsDouble(prhs[0])*/)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   iSize = mxGetNumberOfElements(prhs[0]);";
"   pDblR = mxGetPr(prhs[0]);";
"   pDblI = mxGetPi(prhs[0]);";
"   pOut = mxCreateDoubleMatrix(1, iSize, 1);";
"   mxSetPr(pOut, pDblI);";
"   mxSetPi(pOut, pDblR);";
"   plhs[0] = pOut;";
"}"],"mexswapRI.c");
ilib_mex_build("libmextest",["swapRI","mexswapRI","cmex"], "mexswapRI.c",[]);
exec("loader.sce");

a = 1:8;
b = 8:-1:1;
var1 = a + b * %i;
ref = b + a * %i;
var2 = swapRI(var1);
assert_checkequal(var2, ref);


