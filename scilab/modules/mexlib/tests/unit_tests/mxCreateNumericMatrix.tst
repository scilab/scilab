// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Siddhartha Gairola
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCreateNumericMatrix mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   double* pdblType = NULL;";
"   mxArray* pOut1 = NULL;";
"   mxArray* pOut2 = NULL;";
"";
"   if(nrhs != 1)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   pdblType = mxGetPr(prhs[0]);";
"   pOut1 = mxCreateNumericMatrix(2, 3, (mxClassID)pdblType[0], mxREAL);";
"   pOut2 = mxCreateNumericMatrix(4, 5, (mxClassID)pdblType[0], mxREAL);";
"   plhs[0] = pOut1;";
"   plhs[1] = pOut2;";
"}"],"mexCreateNumericMatrix.c");
ilib_mex_build("libmextest",["createNumericMatrix","mexCreateNumericMatrix","cmex"], "mexCreateNumericMatrix.c",[]);
exec("loader.sce");

[a, b] = createNumericMatrix(8);

[n1, m1] = size(a);
[n2, m2] = size(b);

assert_checkequal([n1, m1], [2, 3]);
assert_checkequal([n2, m2], [4, 5]);

comp1 = zeros(2,3);
comp2 = zeros(4,5);

assert_checkequal(a, int8(comp1));
assert_checkequal(b, int8(comp2));
