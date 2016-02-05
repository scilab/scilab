// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxSetDimentsions mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    int dims[2] = {2,2};";
"    mxSetDimensions(prhs[0], dims, 2);";
"}"],"mexsetDimensions.c");
ilib_mex_build("libmextest",["setDimensions","mexsetDimensions","cmex"], "mexsetDimensions.c",[]);
exec("loader.sce");

a = 3;
setDimensions(a);
assert_checkequal(size(a, "*"), 4);
assert_checkequal(a(1), 3);
