// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxSetField mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    plhs[0] = mxDuplicateArray(prhs[0]);";
"    mxSetField(plhs[0], 0, ""a"", mxCreateDoubleScalar(3));";
"}"],"mexsetField.c");
ilib_mex_build("libmextest",["setField","mexsetField","cmex"], "mexsetField.c",[]);
exec("loader.sce");


s = struct("a", 1, "b", 2);
s2 = setField(s);
assert_checkequal(s2.a, 3);
