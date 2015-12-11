// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetM, mxGetN, mSGetM, mxSetN mex functions
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);

ilib_verbose(0);
mputl([ "#include ""mex.h""";
"#include ""localization.h""";
"";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   int i = 0;";
"   int iRows = 0;";
"   int iCols = 0;";
"";
"   if(nrhs != 1 /*|| !mxIsDouble(prhs[0])*/)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   iRows = mxGetM(prhs[0]);";
"   iCols = mxGetN(prhs[0]);";
"   mxSetM(prhs[0], iRows * 2);";
"   mxSetN(prhs[0], iCols * 2);";
"   plhs[0] = prhs[0];";
"}"],"mexGetSetMN.c");
ilib_mex_build("libmextest",["expand","mexGetSetMN","cmex"], "mexGetSetMN.c",[]);
exec("loader.sce");

a = [1,2;3,4];
ref = a;
ref(4,4) = 0;
b = expand(a);
assert_checkequal(b, ref);
