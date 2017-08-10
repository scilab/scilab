// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Gsoc 2017 - Siddhartha Gairola
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxIsNumeric mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    bool isNumeric = mxIsNumeric(prhs[0]);";
"    mxArray* pOut = mxCreateLogicalScalar(isNumeric);";
"    plhs[0] = pOut;";
"}"],"mexisNumeric.c");

ilib_mex_build("libmextest",["isNumeric","mexisNumeric","cmex"], "mexisNumeric.c",[]);
exec("loader.sce");

res = isNumeric(1);
assert_checktrue(res);

res_int8 = isNumeric(int8(1));
assert_checktrue(res_int8);

res_int16 = isNumeric(int16(1));
assert_checktrue(res_int16);

res_int32 = isNumeric(int32(1));
assert_checktrue(res_int32);

res_int64 = isNumeric(int64(1));
assert_checktrue(res_int64);

res_uint8 = isNumeric(uint8(1));
assert_checktrue(res_uint8);

res_uint16 = isNumeric(uint16(1));
assert_checktrue(res_uint16);

res_uint32 = isNumeric(uint32(1));
assert_checktrue(res_uint32);

res_uint64 = isNumeric(uint64(1));
assert_checktrue(res_uint64);

res_double = isNumeric(2.0);
assert_checktrue(res_double);

res_char = isNumeric("a");
assert_checkfalse(res_char);
