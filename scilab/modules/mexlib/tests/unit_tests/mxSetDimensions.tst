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
"    plhs[0] = mxDuplicateArray(prhs[0]);";
"    mxSetDimensions(plhs[0], dims, 2);";
"}"],"mexsetDimensions.c");
ilib_mex_build("libmextest",["setDimensions","mexsetDimensions","cmex"], "mexsetDimensions.c",[]);
exec("loader.sce");

a = 3;
b = setDimensions(a);
assert_checkequal(size(b, "*"), 4);
assert_checkequal(b(1), 3);
