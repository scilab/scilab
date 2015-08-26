// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCreateNumericArray mex function
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
"";
"   if(nrhs != 1 /*|| !mxIsDouble(prhs[0])*/)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   pdblType = mxGetPr(prhs[0]);";
"   pOut = mxCreateNumericArray(iDims, piDims, (mxClassID)pdblType[0], mxREAL);";
"   plhs[0] = pOut;";
"}"],"mexCreateNumericArray.c");
ilib_mex_build("libmextest",["createNumericArray","mexCreateNumericArray","cmex"], "mexCreateNumericArray.c",[]);
exec("loader.sce");

ret8 = createNumericArray(8);
retU8 = createNumericArray(9);
ret16 = createNumericArray(10);
retU16 = createNumericArray(11);
ret32 = createNumericArray(12);
retU32 = createNumericArray(13);
ret64 = createNumericArray(14);
retU64 = createNumericArray(15);
