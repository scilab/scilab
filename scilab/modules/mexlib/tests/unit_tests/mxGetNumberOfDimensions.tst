// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetNumberOfDimensions mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   int iDims = 0;";
"   mxArray* pOut = NULL;";
"";
"   if(nrhs != 1)";
"   {";
"       mexErrMsgTxt(""Wrong number of input argument"");";
"   }";
"";
"   iDims = mxGetNumberOfDimensions(prhs[0]);";
"   pOut = mxCreateDoubleScalar((double)iDims);";
"   plhs[0] = pOut;";
"}"],"mexGetNumberOfDimensions.c");
ilib_mex_build("libmextest",["getDims","mexGetNumberOfDimensions","cmex"], "mexGetNumberOfDimensions.c",[]);
exec("loader.sce");

a = ones(5,4,3,2);
assert_checkequal(getDims(a), 4);
