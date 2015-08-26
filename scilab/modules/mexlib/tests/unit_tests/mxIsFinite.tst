// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxIsFinite mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
ieee(2);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    double value = mxGetScalar(prhs[0]);";
"    bool isFinite = mxIsFinite(value);";
"    mxArray* pOut = mxCreateLogicalScalar(isFinite);";
"    plhs[0] = pOut;";
"}"],"mexisFinite.c");
ilib_mex_build("libmextest",["isFinite","mexisFinite","cmex"], "mexisFinite.c",[]);
exec("loader.sce");

assert_checkfalse(isFinite(%inf));
assert_checkfalse(isFinite(1/0));
assert_checktrue(isFinite(100));
assert_checktrue(isFinite(0));
assert_checkfalse(isFinite(%nan));
