// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxSetFieldByNumber mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mxSetFieldByNumber(prhs[0], 0, 0, mxCreateDoubleScalar(3));";
"    mxSetFieldByNumber(prhs[0], 0, 1, mxCreateDoubleScalar(6));";
"    plhs[0] = prhs[0];";
"}"],"mexsetFieldByNumber.c");
ilib_mex_build("libmextest",["setFieldByNumber","mexsetFieldByNumber","cmex"], "mexsetFieldByNumber.c",[]);
exec("loader.sce");

s = struct("a", 1, "b", 2);
r = setFieldByNumber(s);
assert_checkequal(r("a"), 3);
assert_checkequal(r("b"), 6);
