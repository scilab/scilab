// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// ============================================================================
// Unitary tests for mxGetClassName mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mexPrintf(mxGetClassName(prhs[0]));";
"}"],"mexgetClassName.c");

ilib_mex_build("libmextest",["printClassName","mexgetClassName","cmex"], "mexgetClassName.c",[]);
exec("loader.sce");

printClassName(cell());
printClassName("s");
printClassName(1);
printClassName(int8(1));
printClassName(int16(1));
printClassName(int32(1));
printClassName(int64(1));
printClassName(%t);
printClassName(struct());
printClassName(uint8(1));
printClassName(uint16(1));
printClassName(uint32(1));
printClassName(uint64(1));
