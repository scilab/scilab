// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetImagData and mxSetImagData mex functions
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    void *data = mxGetImagData(prhs[0]);";
"    plhs[0] = mxDuplicateArray(prhs[0]);";
"    mxSetImagData(plhs[0], data);";
"}"],"mexGetSetImagData.c");
ilib_mex_build("libmextest",["getSetImagData","mexGetSetImagData","cmex"], "mexGetSetImagData.c",[]);
exec("loader.sce");

r = getSetImagData(complex(1, 2));
assert_checkequal(r(1), complex(1, 2));
