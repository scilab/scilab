// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxIsComplex mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    bool isComplex = mxIsComplex(prhs[0]);";
"    mxArray* pOut = mxCreateLogicalScalar(isComplex);";
"    plhs[0] = pOut;";
"}"],"mexisComplex.c");

ilib_mex_build("libmextest",["isComplex","mexisComplex","cmex"], "mexisComplex.c",[]);
exec("loader.sce");

assert_checkfalse(isComplex(1));
assert_checktrue(isComplex(1+0*%i));
assert_checktrue(isComplex(2+1*%i));
assert_checktrue(isComplex(complex(1, 1)));
