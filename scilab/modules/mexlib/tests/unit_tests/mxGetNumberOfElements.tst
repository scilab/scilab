// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetNumberOfElements mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   int iSize = 0;";
"   mxArray* pOut = NULL;";
"";
"   if(nrhs != 1)";
"   {";
"       mexErrMsgTxt(""Wrong number of input argument"");";
"   }";
"";
"   iSize = mxGetNumberOfElements(prhs[0]);";
"   pOut = mxCreateDoubleScalar((double)iSize);";
"   plhs[0] = pOut;";
"}"],"mexGetNumberOfElements.c");
ilib_mex_build("libmextest",["getNumberOfElements","mexGetNumberOfElements","cmex"], "mexGetNumberOfElements.c",[]);
exec("loader.sce");

a = ones(1);
assert_checkequal(getNumberOfElements(a), size(a, "*"));

a = ones(1, 2);
assert_checkequal(getNumberOfElements(a), size(a, "*"));

a = ones(1, 2, 3);
assert_checkequal(getNumberOfElements(a), size(a, "*"));

a = ones(1, 2, 3, 4);
assert_checkequal(getNumberOfElements(a), size(a, "*"));

