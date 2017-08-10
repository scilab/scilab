// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCalcSingleSubscript mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    int *dims = mxGetData(prhs[2]);";
"    int ndim = mxGetScalar(prhs[1]);";
"    plhs[0] = mxCreateDoubleScalar(mxCalcSingleSubscript(prhs[0], ndim, dims));";
"}"],"mexcalcSingleSubscript.c");
ilib_mex_build("libmextest", ["calcSingleSubscript", "mexcalcSingleSubscript", "cmex"], "mexcalcSingleSubscript.c", []);
exec("loader.sce");

a = zeros(3, 3);
r = calcSingleSubscript(a, 2, int32([0 0]));
assert_checkequal(r, 0);
r = calcSingleSubscript(a, 2, int32([1 1]));
assert_checkequal(r, 4);
r = calcSingleSubscript(a, 2, int32([2 2]));
assert_checkequal(r, 8);

b = zeros(4, 2, 3);
r = calcSingleSubscript(b, 3, int32([0 0 0]));
assert_checkequal(r, 0);
r = calcSingleSubscript(b, 2, int32([0 0]));
assert_checkequal(r, 0);
r = calcSingleSubscript(b, 3, int32([3 0 2]));
assert_checkequal(r, 19);
r = calcSingleSubscript(b, 3, int32([1 1 1]));
assert_checkequal(r, 13);
r = calcSingleSubscript(b, 3, int32([3 1 2]));
assert_checkequal(r, 23);
