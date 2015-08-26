// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - Gsoc 2011 - Iuri SILVIO
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//
// ============================================================================
// Unitary tests for mxDestroyArray mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl(["#include ""mex.h""";
"void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])";
"{";
"    mxArray *d = mxDuplicateArray(prhs[0]);"
"    mxDestroyArray(d);";
"    if (*d == NULL)";
"    {";
"        mexPrintf(""It works."");";
"    }";
"}"],"mexdestroyArray.c");

ilib_mex_build("libmextest",["destroyArray","mexdestroyArray","cmex"], "mexdestroyArray.c",[]);
exec("loader.sce");

destroyArray(cell());
destroyArray("s");
destroyArray(1);
destroyArray(int8(1));
destroyArray(int16(1));
destroyArray(int32(1));
destroyArray(int64(1));
destroyArray(%t);
destroyArray(struct());
destroyArray(uint8(1));
destroyArray(uint16(1));
destroyArray(uint32(1));
destroyArray(uint64(1));
