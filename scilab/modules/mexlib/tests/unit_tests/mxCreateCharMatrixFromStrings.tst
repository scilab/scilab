// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCreateCharMatrixFromStrings mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    char *strings[2];";
"    strings[0] = ""value"";";
"    strings[1] = ""another value"";";
"    mxArray *pOut = mxCreateCharMatrixFromStrings(2, strings);";
"    plhs[0] = pOut;";
"}"],"mexcreateCharMatrixFromStrings.c");
ilib_mex_build("libmextest", ["createCharMatrixFromStrings", "mexcreateCharMatrixFromStrings", "cmex"], "mexcreateCharMatrixFromStrings.c", []);
exec("loader.sce");

res = createCharMatrixFromStrings();
assert_checkequal(res(1), "value");
assert_checkequal(res(2), "another value");