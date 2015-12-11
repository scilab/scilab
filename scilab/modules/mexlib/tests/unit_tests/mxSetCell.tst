// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxSetCell mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mxArray *pOut = mxCreateCellMatrix(3, 3);";
"    mxSetCell(pOut, 0, mxCreateDoubleScalar(1));";
"    mxSetCell(pOut, 5, mxCreateDoubleScalar(9));";
"    mxSetCell(pOut, 7, mxGetCell(pOut, 5));";
"    plhs[0] = pOut;";
"}"],"mexsetCell.c");
ilib_mex_build("libmextest",["setCell","mexsetCell","cmex"], "mexsetCell.c",[]);
exec("loader.sce");

out = setCell();
assert_checkequal(size(out), [3,3]);

ref = {1,[],[];[],[],9;[],9,[]};
assert_checkequal(out, ref);
