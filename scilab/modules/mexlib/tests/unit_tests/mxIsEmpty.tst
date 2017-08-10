// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxIsEmpty mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   int bEmpty = 0;";
"   mxArray* pOut = NULL;";
"";
"   if(nrhs != 1 /*|| !mxIsDouble(prhs[0])*/)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   bEmpty = mxIsEmpty(prhs[0]);";
"   pOut = mxCreateLogicalScalar(bEmpty);";
"   plhs[0] = pOut;";
"}"],"mexIsEmpty.c");
ilib_mex_build("libmextest",["isEmpty","mexIsEmpty","cmex"], "mexIsEmpty.c",[]);
exec("loader.sce");

assert_checktrue(isEmpty(list()));
assert_checkfalse(isEmpty(list(1)));
assert_checktrue(isEmpty({}));
assert_checkfalse(isEmpty({1}));
assert_checktrue(isEmpty([]));
assert_checkfalse(isEmpty(1));
assert_checkfalse(isEmpty(""));


