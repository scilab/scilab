// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017-2017 - Gsoc 2017 - Siddhartha Gairola
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxIsScalar mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
ieee(2);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    bool isScalar = mxIsScalar(prhs[0]);";
"    mxArray* pOut = mxCreateLogicalScalar(isScalar);";
"    plhs[0] = pOut;";
"}"],"mexisScalar.c");
ilib_mex_build("libmextest",["isScalar","mexisScalar","cmex"], "mexisScalar.c",[]);
exec("loader.sce");

assert_checktrue(isScalar([1]));
assert_checktrue(isScalar(1/0));
assert_checktrue(isScalar(9));
