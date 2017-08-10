// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCreateLogicalMatrix mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   double* pdblM = NULL;";
"   double* pdblN = NULL;";
"   mxArray* pOut = NULL;";
"";
"   if(nrhs != 2 /*|| !mxIsDouble(prhs[0])*/)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   pdblM = mxGetPr(prhs[0]);";
"   pdblN = mxGetPr(prhs[1]);";
"   pOut = mxCreateLogicalMatrix((int)pdblM[0], (int)pdblN[0]);";
"   plhs[0] = pOut;";
"}"],"mexCreateLogicalMatrix.c");
ilib_mex_build("libmextest",["createLogicalMatrix", "mexCreateLogicalMatrix", "cmex"], "mexCreateLogicalMatrix.c", []);
exec("loader.sce");

a = createLogicalMatrix(4,3);
assert_checkequal(size(a, "*"), 12);
assert_checkequal(typeof(a), "boolean");
