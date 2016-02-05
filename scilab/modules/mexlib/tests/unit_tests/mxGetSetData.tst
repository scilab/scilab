// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetData and mxSetData mex functions
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    void *data = mxGetData(prhs[0]);";
"    mxSetData(prhs[0], data);";
"    plhs[0] = prhs[0];";
"}"],"mexGetSetData.c");
ilib_mex_build("libmextest",["getSetData","mexGetSetData","cmex"], "mexGetSetData.c",[]);
exec("loader.sce");

assert_checkequal(getSetData(double(1)), 1);
assert_checkequal(getSetData(int8(2)), int8(2));
assert_checkequal(getSetData(int16(3)), int16(3));
assert_checkequal(getSetData(int32(4)), int32(4));
assert_checkequal(getSetData(int64(5)), int64(5));
assert_checkequal(getSetData(uint8(6)), uint8(6));
assert_checkequal(getSetData(uint16(7)), uint16(7));
assert_checkequal(getSetData(uint32(8)), uint32(8));
assert_checkequal(getSetData(uint64(9)), uint64(9));
assert_checktrue(getSetData(%t));
