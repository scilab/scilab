// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCreateStructMatrix and mxCreateStructArray mex functions
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    int dims[2] = {3, 3};";
"    char *fields[2];";
"    fields[0] = ""f1"";";
"    fields[1] = ""f2"";";
"    plhs[0] = mxCreateStructArray(2, dims, 2, fields);";
"    plhs[1] = mxCreateStructMatrix(2, 2, 2, fields);";
"}"],"mexcreateStruct.c");
ilib_mex_build("libmextest",["createStruct","mexcreateStruct","cmex"], "mexcreateStruct.c",[]);
exec("loader.sce");

[out1, out2] = createStruct();

assert_checkequal(size(out1, "*"), 9);

assert_checkequal(size(out2, "*"), 4);
