// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
// ============================================================================
// Unitary tests for mxAssertS mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mxAssertS(mxGetScalar(prhs[0]), mxGetChars(prhs[1]));";
"}"],"mexAssertS.c");
ilib_mex_build("libmextest",["assertS","mexAssertS","cmex"], "mexAssertS.c",[]);
exec("loader.sce");

assertS(1, "This is true");
assertS(0, "This is false and throws an exception.");
assertS(1, "This assert is never executed");
