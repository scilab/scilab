// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetNumberOfFields mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    int num = mxGetNumberOfFields(prhs[0]);";
"    plhs[0] = mxCreateDoubleScalar(num);";
"}"],"mexgetNumberOfFields.c");
ilib_mex_build("libmextest",["getNumberOfFields","mexgetNumberOfFields","cmex"], "mexgetNumberOfFields.c",[]);
exec("loader.sce");

c = struct("a", 5, "b", 3, "e", 1, "another key", "hello");
assert_checkequal(getNumberOfFields(c), 4);

c = struct("a", 5, "b", 3);
assert_checkequal(getNumberOfFields(c), 2);

c = struct("b", 32);
assert_checkequal(getNumberOfFields(c), 1);

r = getNumberOfFields("not_a_struct");
assert_checkequal(r, 0);
