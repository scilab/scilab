// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxIsCell, mxIsChar, mxIsDouble, mxIsFunction,
// mxIsInt8, mxIsInt16, mxIsInt32, mxIsInt64,
// mxIsUint8, mxIsUint16, mxIsUint32, mxIsUint64,
// mxIsLogical, mxIsStruct and mxIsSingle mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    if (mxIsCell(prhs[0])) {";
"        mexPrintf(""cell"");";
"    }"
"    if (mxIsChar(prhs[0])) {";
"        mexPrintf(""char"");";
"    }"
"    if (mxIsDouble(prhs[0])) {";
"        mexPrintf(""double"");";
"    }"
"    if (mxIsInt8(prhs[0])) {";
"        mexPrintf(""int8"");";
"    }"
"    if (mxIsInt16(prhs[0])) {";
"        mexPrintf(""int16"");";
"    }"
"    if (mxIsInt32(prhs[0])) {";
"        mexPrintf(""int32"");";
"    }"
"    if (mxIsInt64(prhs[0])) {";
"        mexPrintf(""int64"");";
"    }"
"    if (mxIsLogical(prhs[0])) {";
"        mexPrintf(""logical"");";
"    }"
"    if (mxIsSingle(prhs[0])) {";
"        mexPrintf(""single"");";
"    }"
"    if (mxIsStruct(prhs[0])) {";
"        mexPrintf(""struct"");";
"    }"
"    if (mxIsUint8(prhs[0])) {";
"        mexPrintf(""uint8"");";
"    }"
"    if (mxIsUint16(prhs[0])) {";
"        mexPrintf(""uint16"");";
"    }"
"    if (mxIsUint32(prhs[0])) {";
"        mexPrintf(""uint32"");";
"    }"
"    if (mxIsUint64(prhs[0])) {";
"        mexPrintf(""uint64"");";
"    }"
"}"],"mexcheckTypes.c");

ilib_mex_build("libmextest",["printClass","mexcheckTypes","cmex"], "mexcheckTypes.c",[]);
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
