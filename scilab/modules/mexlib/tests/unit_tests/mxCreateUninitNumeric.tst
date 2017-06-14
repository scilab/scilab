// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Gsoc 2017 - Siddhartha Gairola
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCreateUninitNumericArray and mxCreateUninitNumericMatrix mex functions
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   int piDims[3] = {4,3,2};";
"   int iDims = 3;";
"   mxClassID CLASS = 0;";
"   double* pdblType = NULL;";
"   mxArray* pOut = NULL;";
"   mxArray* pOut1 = NULL;";
"   mxArray* pOut2 = NULL;";
"";
"   if(nrhs != 1)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   pdblType = mxGetPr(prhs[0]);";
"   pOut = mxCreateUninitNumericArray(iDims, piDims, (mxClassID)pdblType[0], mxREAL);";
"   pOut1 = mxCreateUninitNumericMatrix(2, 3, (mxClassID)pdblType[0], mxREAL);";
"   pOut2 = mxCreateUninitNumericMatrix(4, 5, (mxClassID)pdblType[0], mxREAL);";
"   plhs[0] = pOut;";
"   plhs[1] = pOut1;";
"   plhs[2] = pOut2;";
"}"],"mexCreateUninitNumeric.c");
ilib_mex_build("libmextest",["createUninitNumeric","mexCreateUninitNumeric","cmex"], "mexCreateUninitNumeric.c",[]);
exec("loader.sce");

[ret8, a, b] = createUninitNumeric(8);
assert_checkequal([2, 3], size(a));
assert_checkequal([4, 5], size(b));
assert_checkequal(ret8, int8(ret8));
[retU8, a, b] = createUninitNumeric(9);
assert_checkequal([2, 3], size(a));
assert_checkequal([4, 5], size(b));
assert_checkequal(retU8, uint8(retU8));
[ret16, a, b] = createUninitNumeric(10);
assert_checkequal([2, 3], size(a));
assert_checkequal([4, 5], size(b));
assert_checkequal(ret16, int16(ret16));
[retU16, a, b] = createUninitNumeric(11);
assert_checkequal([2, 3], size(a));
assert_checkequal([4, 5], size(b));
assert_checkequal(retU16, uint16(retU16));
[ret32, a, b] = createUninitNumeric(12);
assert_checkequal([2, 3], size(a));
assert_checkequal([4, 5], size(b));
assert_checkequal(ret32, int32(ret32));
[retU32, a, b] = createUninitNumeric(13);
assert_checkequal([2, 3], size(a));
assert_checkequal([4, 5], size(b));
assert_checkequal(retU32, uint32(retU32));
[ret64, a, b] = createUninitNumeric(14);
assert_checkequal([2, 3], size(a));
assert_checkequal([4, 5], size(b));
assert_checkequal(ret64, int64(ret64));
[retU64, a, b] = createUninitNumeric(15);
assert_checkequal([2, 3], size(a));
assert_checkequal([4, 5], size(b));
assert_checkequal(retU64, uint64(retU64));
