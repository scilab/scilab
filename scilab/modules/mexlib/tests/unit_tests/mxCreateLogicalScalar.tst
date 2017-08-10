// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCreateLogicalScalar mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   int bIn1 = 0;";
"   int bIn2 = 0;";
"   mxArray* pOut1 = NULL;";
"   mxArray* pOut2 = NULL;";
"";
"   if(nrhs != 2 || nlhs != 2/*|| !mxIsDouble(prhs[0])*/)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   bIn1 = mxIsLogicalScalarTrue(prhs[0]);";
"   bIn2 = mxIsLogicalScalarTrue(prhs[1]);";
"";
"   pOut1 = mxCreateLogicalScalar(!bIn1);";
"   pOut2 = mxCreateLogicalScalar(!bIn2);";
"";
"   plhs[0] = pOut1;";
"   plhs[1] = pOut2;";
"}"],"mexCreateLogicalScalar.c");
ilib_mex_build("libmextest", ["createLogicalScalar", "mexCreateLogicalScalar", "cmex"], "mexCreateLogicalScalar.c", []);
exec("loader.sce");

[out1, out2] = createLogicalScalar(%t, %f);

assert_checkfalse(out1);
assert_checktrue(out2);

