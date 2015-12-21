// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//
// ============================================================================
// Unitary tests for mexIsGlobal mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    plhs[0] = mxCreateLogicalScalar(mexIsGlobal(prhs[0]));";
"}"],"mexisGlobal.c");
ilib_mex_build("libmextest", ["isGlobal", "mexisGlobal", "cmex"], "mexisGlobal.c", []);
exec("loader.sce");

global globalvar;
globalvar = 1;
localvar = "s";
assert_checktrue(isGlobal(globalvar));
assert_checkfalse(isGlobal(localvar));
