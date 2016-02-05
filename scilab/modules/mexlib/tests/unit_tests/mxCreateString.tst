// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxCreateString mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ "#include ""mex.h""";
"";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"   mxArray* pOut = NULL;";
"";
"   if(nrhs != 0 /*|| !mxIsDouble(prhs[0])*/)";
"   {";
"       mexErrMsgTxt(""Wrong number or type of input argument"");";
"   }";
"";
"   pOut = mxCreateString(""Create new String"");";
"   plhs[0] = pOut;";
"}"],"mexCreateString.c");
ilib_mex_build("libmextest",["createString","mexCreateString","cmex"], "mexCreateString.c",[]);
exec("loader.sce");

assert_checkequal(createString(), "Create new String");
