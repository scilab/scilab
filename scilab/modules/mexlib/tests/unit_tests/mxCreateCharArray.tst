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
// Unitary tests for mxCreateCharArray mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"#include <wchar.h>";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    int dims[2] = {1, 1};";
"    mxArray *pOut1 = mxCreateCharArray(2, dims);";
"    plhs[0] = pOut1;";
"    wchar_t *data[1];";
"    data[0] = L""hello world"";";
"    mxArray *pOut2 = mxCreateCharArray(2, dims);";
"    mxSetData(pOut2, data);";
"    plhs[1] = pOut2;";
"}"],"mexcreateCharArray.c");
ilib_mex_build("libmextest",["createCharArray","mexcreateCharArray","cmex"], "mexcreateCharArray.c",[]);
exec("loader.sce");

[out1, out2] = createCharArray();
assert_checkequal(size(out1, "*"), 1);
assert_checkequal(out2, "hello world");
