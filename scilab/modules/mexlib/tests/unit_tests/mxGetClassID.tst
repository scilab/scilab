// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetClassID mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    if (mxGetClassID(prhs[0]) == mxCELL_CLASS) {";
"        mexPrintf(""cell"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxCHAR_CLASS) {";
"        mexPrintf(""char"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxDOUBLE_CLASS) {";
"        mexPrintf(""double"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxFUNCTION_CLASS) {";
"        mexPrintf(""function_handle"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxINT8_CLASS) {";
"        mexPrintf(""int8"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxINT16_CLASS) {";
"        mexPrintf(""int16"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxINT32_CLASS) {";
"        mexPrintf(""int32"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxINT64_CLASS) {";
"        mexPrintf(""int64"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxLOGICAL_CLASS) {";
"        mexPrintf(""logical"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxSINGLE_CLASS) {";
"        mexPrintf(""single"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxSTRUCT_CLASS) {";
"        mexPrintf(""struct"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxUINT8_CLASS) {";
"        mexPrintf(""uint8"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxUINT16_CLASS) {";
"        mexPrintf(""uint16"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxUINT32_CLASS) {";
"        mexPrintf(""uint32"");";
"    }"
"    if (mxGetClassID(prhs[0]) == mxUINT64_CLASS) {";
"        mexPrintf(""uint64"");";
"    }"
"}"],"mexgetClassID.c");

ilib_mex_build("libmextest",["printClass","mexgetClassID","cmex"], "mexgetClassID.c",[]);
exec("loader.sce");

printClass(cell());
printClass("s");
printClass(1);
printClass(sum);
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
