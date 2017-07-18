// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
// Copyright (C) 2017-2017 - Gsoc 2017 - Siddhartha Gairola
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
//checking if it works for sparse matrices
sp = sparse([1]);
sp2 = sparse([1 2]);
c = setDimensions(sp);
d = setDimensions(sp2);
assert_checkequal(size(c, "*"), 4);
assert_checkequal(size(c), [2 2]);
assert_checkequal(size(d), [2 2]);
