// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017-2017 - Gsoc 2017 - Siddhartha Gairola
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxArrayToUTF8String mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    char *str = mxArrayToUTF8String(prhs[0]);";
"    bool check = 0;";
"    mexPrintf(""%s"", str);";
"    if (strcmp(str, ""hello world"") == 0 || strcmp(str, ""twolines"") == 0)";
"       check = 1;";
"    mxArray* pOut = mxCreateLogicalScalar(check);";
"    plhs[0] = pOut;";
"}"],"mexArrayToUTF8String.c");
ilib_mex_build("libmextest", ["arrayToUTF8String", "mexArrayToUTF8String", "cmex"], "mexArrayToUTF8String.c", []);
exec("loader.sce");

res1 = arrayToUTF8String("hello world");
res2 = arrayToUTF8String(["two"; "lines"]);
res3 = arrayToUTF8String(["random"]);

assert_checktrue(res1);
assert_checktrue(res2);
assert_checkfalse(res3);
