// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetElementSize mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"#include ""wchar.h""";
"void check(mxArray *ptr, int size)";
"{";
"    if (mxGetElementSize(ptr) == size)";
"    {"
"        mexPrintf(""T"");";
"    }"
"    else"
"    {"
"        mexPrintf(""F"");";
"    }"
"}";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    check(mxCreateCellMatrix(2, 2), sizeof(mxArray *));";
"    check(mxCreateStructMatrix(2, 2, 0, NULL), sizeof(mxArray *));";
"    check(mxCreateString(""foo""), sizeof(wchar_t *));";
"    check(mxCreateDoubleScalar(1), sizeof(double));";
"    check(mxCreateNumericMatrix(2, 2, mxINT8_CLASS, mxREAL), sizeof(char));";
"    check(mxCreateNumericMatrix(2, 2, mxINT16_CLASS, mxREAL), sizeof(short));";
"    check(mxCreateNumericMatrix(2, 2, mxINT32_CLASS, mxREAL), sizeof(int));";
"    check(mxCreateNumericMatrix(2, 2, mxINT64_CLASS, mxREAL), sizeof(long long));";
"    check(mxCreateNumericMatrix(2, 2, mxUINT8_CLASS, mxREAL), sizeof(unsigned char));";
"    check(mxCreateNumericMatrix(2, 2, mxUINT16_CLASS, mxREAL), sizeof(unsigned short));";
"    check(mxCreateNumericMatrix(2, 2, mxUINT32_CLASS, mxREAL), sizeof(unsigned int));";
"    check(mxCreateNumericMatrix(2, 2, mxUINT64_CLASS, mxREAL), sizeof(unsigned long long));";
"    check(mxCreateLogicalScalar(0), sizeof(int));";
"}"],"mexgetElementSize.c");


ilib_mex_build("libmextest",["getElementSize","mexgetElementSize","cmex"], "mexgetElementSize.c",[]);
exec("loader.sce");

getElementSize();
