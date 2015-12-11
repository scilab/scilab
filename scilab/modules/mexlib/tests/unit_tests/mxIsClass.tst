// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxIsClass mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    if (mxIsClass(prhs[0], ""cell"")) {";
"        mexPrintf(""cell\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""char"")) {";
"        mexPrintf(""char\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""double"")) {";
"        mexPrintf(""double\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""int8"")) {";
"        mexPrintf(""int8\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""int16"")) {";
"        mexPrintf(""int16\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""int32"")) {";
"        mexPrintf(""int32\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""int64"")) {";
"        mexPrintf(""int64\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""logical"")) {";
"        mexPrintf(""logical\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""single"")) {";
"        mexPrintf(""single\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""struct"")) {";
"        mexPrintf(""struct\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""uint8"")) {";
"        mexPrintf(""uint8\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""uint16"")) {";
"        mexPrintf(""uint16\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""uint32"")) {";
"        mexPrintf(""uint32\n"");";
"    }"
"    if (mxIsClass(prhs[0], ""uint64"")) {";
"        mexPrintf(""uint64\n"");";
"    }"
"}"],"mexisClass.c");

ilib_mex_build("libmextest",["printClass","mexisClass","cmex"], "mexisClass.c",[]);
exec("loader.sce");

printClass(cell());
printClass("s");
printClass(1);
printClass(int8(1));
printClass(int16(1));
printClass(int32(1));
printClass(int64(1));
printClass(%t);
printClass(struct());
printClass(uint8(1));
printClass(uint16(1));
printClass(uint32(1));
printClass(uint64(1));
