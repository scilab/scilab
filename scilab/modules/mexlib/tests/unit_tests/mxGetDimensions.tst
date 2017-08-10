// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetDimensions mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    int ndim = mxGetNumberOfDimensions(prhs[0]);";
"    int *dims = mxGetDimensions(prhs[0]);";
"    mxArray *ptr = mxCreateNumericMatrix(1, ndim, mxINT32_CLASS, mxREAL);";
"    mxSetPr(ptr, dims);";
"    plhs[0] = ptr;";
"}"],"mexgetDimensions.c");
ilib_mex_build("libmextest",["getDimensions","mexgetDimensions","cmex"], "mexgetDimensions.c",[]);
exec("loader.sce");

assert_checkequal(getDimensions([1]), int32([1 1]));
assert_checkequal(getDimensions([1 1 1]), int32([1 3]));
assert_checkequal(getDimensions([1;1;1]), int32([3 1]));
assert_checkequal(getDimensions([1 1;2 2;3 3]), int32([3 2]));
assert_checkequal(getDimensions(zeros(4, 2, 3)), int32([4 2 3]));
