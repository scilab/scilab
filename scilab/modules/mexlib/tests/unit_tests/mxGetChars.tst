// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetChars mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    char *chars = mxGetChars(prhs[0]);";
"    plhs[0] = mxCreateString(chars);";
"}"],"mexgetChars.c");
ilib_mex_build("libmextest",["getChars","mexgetChars","cmex"], "mexgetChars.c",[]);
exec("loader.sce");

assert_checkequal(getChars("hello world"), "hello world");
